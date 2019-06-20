#include "FractalFrame.h"

#include "HDPrintscreenDialog.h"
#include "CURSOR_CROSS.h"

#include <chrono>
#include <fstream>
#include <mutex>

///Event enumeration
enum{
    ID_PRINTSCREEN    = 1,
    ID_HDPRINTSCREEN  = 2
};
///Constructor
FractalFrame::FractalFrame(FractalBitmap *p):wxFrame(nullptr, wxID_ANY, "Mandelbrot set plotter"){
    /**Cursor*/{
        SetCursor(CURSOR_CROSS);
    }
    /**Menu*/{
        wxMenu* menuFile      = new wxMenu;
        wxMenuItem* menuItem_Printscreen   = new wxMenuItem(menuFile, ID_PRINTSCREEN  , wxT("Save printscreen"));
        wxMenuItem* menuItem_HDPrintscreen = new wxMenuItem(menuFile, ID_HDPRINTSCREEN, wxT("Save HD printscreen"));
        menuFile->Append(menuItem_Printscreen  );
        menuFile->Append(menuItem_HDPrintscreen);

        wxMenuBar* menuBar = new wxMenuBar;
        menuBar->Append(menuFile, "File");
        this->SetMenuBar(menuBar);
    }
    /**Panels*/{
        fpanel = new FractalPanel(this, wxSize(1150, 500), p);
        ipanel = new InfoPanel   (this);
    }
    /**Background color*/{
        SetBackgroundColour(ipanel->GetBackgroundColour());
    }
    /**Sizers*/{
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(fpanel, 1, wxALL | wxEXPAND, 5);
        sizer->Add(ipanel, 0, wxLEFT | wxBOTTOM | wxRIGHT | wxEXPAND, 7);
        sizer->SetSizeHints(this);
        this->SetSizer(sizer);
    }
    /**Create fractal thread*/{
        if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR){
            wxLogError("Could not create main thread"); return;
        }
        if (GetThread()->Run() != wxTHREAD_NO_ERROR){
            wxLogError("Could not run main thread"   ); return;
        }
    }
}

typedef std::chrono::high_resolution_clock hrclock;
wxThread::ExitCode FractalFrame::Entry(){
    /**Create fractal*/{
        fpanel->f->New(FractalBitmap::ComplexNum(FractalBitmap::ComplexT(-0.75),FractalBitmap::ComplexT(0.0L)),
            FractalHeight*FractalBitmap::ComplexT(1.0L/0.8L/GetSize().y), fpanel->GetSize(), true);
    }
    while(true){
        {
            std::lock_guard<std::mutex> lock(fpanel->f->Mutex);
            ///Update the fractal & measure time
            auto t1 = hrclock::now();
            fpanel->f->UpdateMath();
            auto t2 = hrclock::now();
            dt = std::chrono::duration<long double>(t2-t1).count();
        }
        ///Update the screen
        wxPaintEvent p;
        ProcessEvent(p);
    }
    return (wxThread::ExitCode)0;
}

void FractalFrame::UpdateInfoPanel(){
    wxPoint p = wxGetMousePosition() - fpanel->GetScreenPosition();
    FractalBitmap::ComplexNum c = fpanel->f->GetOrigin() + FractalBitmap::ComplexNum(+(FractalBitmap::ComplexT)p.x*fpanel->f->GetStep(),-(FractalBitmap::ComplexT)p.y*fpanel->f->GetStep());
    ipanel->Update(c, fpanel->f->GetStep(), fpanel->f->GetNum(), dt/fpanel->f->GetCyclesPerRun(), fpanel->f->GetHorizontalSize());
}

void FractalFrame::OnZoomEvent(wxMouseEvent& evt){
    std::lock_guard<std::mutex> lock(fpanel->f->Mutex);

    wxPoint p = wxGetMousePosition() - fpanel->GetScreenPosition();
    FractalBitmap::ComplexNum newcenter = fpanel->f->GetOrigin() + FractalBitmap::ComplexNum(
        FractalBitmap::ComplexT(p.x)*fpanel->f->GetStep(), FractalBitmap::ComplexT(-p.y)*fpanel->f->GetStep()
    );
    FractalBitmap::ComplexT newstep = fpanel->f->GetStep()*FractalBitmap::ComplexT(evt.GetWheelRotation() < 0 ? 3.16227766017L : 1.0L/3.16227766017L);
    fpanel->f->New(newcenter, newstep, fpanel->GetSize(), true);
}

void NewImageName(const char* format, char* name){
    const unsigned long N = 100000;
    for(unsigned long i = 0; i < N; ++i){
        sprintf(name, format, i);
        if(!std::ifstream(name)) return;
    }
    throw std::logic_error("no available names");
}
void FractalFrame::OnPrintscreenEvent(wxCommandEvent &event){
    std::lock_guard<std::mutex> lock(fpanel->f->Mutex);

    char new_path[256];
    NewImageName(".\\Printscreens\\Image_%04d.png", new_path);
    if(fpanel->f->SaveFile(new_path, wxBITMAP_TYPE_PNG))
        wxLogMessage("Printscreen saved as " + wxString(new_path));
}
void FractalFrame::OnHDPrintscreenEvent(wxCommandEvent &event){
    std::lock_guard<std::mutex> lock(fpanel->f->Mutex);

    char new_path[256];
    NewImageName(".\\Printscreens\\Image_%04d.png", new_path);

    FractalBitmap::ComplexNum center = fpanel->f->GetCenter();
    FractalBitmap::ComplexT step = fpanel->f->GetStep();
    wxSize sz = fpanel->f->GetSize();
    FractalBitmap::IterationT num = fpanel->f->GetNum();
    HDPrintscreenDialog *dialog = new HDPrintscreenDialog(this, &center, &step, &sz, &num);
    if(dialog->ShowModal() != wxID_OK) return;
    FractalBitmap *g = fpanel->f->CreateNew(center, step, sz, true);

    num /= g->GetCyclesPerRun();
    while(--num){
        g->UpdateMath();
    }
    if(g->SaveFile(new_path, wxBITMAP_TYPE_PNG))
        wxLogMessage("Printscreen saved as " + wxString(new_path));
}

void FractalFrame::OnPaintEvent(wxPaintEvent& p){
    std::lock_guard<std::mutex> lock(fpanel->f->Mutex);
    wxClientDC dc(fpanel);
    dc.DrawBitmap(*((wxBitmap*)(fpanel->f)), 0, 0, true);
    ///Update the InfoPanel
    CallAfter(&FractalFrame::UpdateInfoPanel);
}

///MACROS - redirect events to functions
wxBEGIN_EVENT_TABLE(FractalFrame, wxFrame)
    EVT_MOUSEWHEEL(FractalFrame::OnZoomEvent)
    EVT_MENU(ID_PRINTSCREEN  , FractalFrame::OnPrintscreenEvent  )
    EVT_MENU(ID_HDPRINTSCREEN, FractalFrame::OnHDPrintscreenEvent)
    EVT_PAINT(FractalFrame::OnPaintEvent                         )
wxEND_EVENT_TABLE()
