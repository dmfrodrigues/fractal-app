#include "HDPrintscreenDialog.h"
#include "myConversions.h"

///Constructor
HDPrintscreenDialog::HDPrintscreenDialog(wxWindow *p, FractalBitmap::ComplexNum *c, FractalBitmap::complex_t *z, wxSize *s, FractalBitmap::iter_t *it)
    :wxDialog(p, wxID_ANY, "Set printscreen settings"), center(c), step(z), sz(s), numIt(it){
    ///Text
    wxStaticText*   ReStat = new wxStaticText(this, wxID_ANY,                  "Real part:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                    ReCtrl = new wxTextCtrl  (this, wxID_ANY, float2str(center->real(), 20), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    wxStaticText*   ImStat = new wxStaticText(this, wxID_ANY,             "Imaginary part:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                    ImCtrl = new wxTextCtrl  (this, wxID_ANY, float2str(center->imag(), 20), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    wxStaticText* StepStat = new wxStaticText(this, wxID_ANY,                       "Step:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                  StepCtrl = new wxTextCtrl  (this, wxID_ANY,           float2str(*step, 8), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    wxStaticText*  Sz1Stat = new wxStaticText(this, wxID_ANY,           "Image dimensions:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                   SzXCtrl = new wxTextCtrl  (this, wxID_ANY, std::to_string(sz->x)        , wxDefaultPosition, wxSize( 70,27),  wxALIGN_LEFT);
    wxStaticText*  Sz2Stat = new wxStaticText(this, wxID_ANY,                         " x ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                   SzYCtrl = new wxTextCtrl  (this, wxID_ANY, std::to_string(sz->y)        , wxDefaultPosition, wxSize( 70,27),  wxALIGN_LEFT);
    wxStaticText*   ItStat = new wxStaticText(this, wxID_ANY,           "Number of cycles:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                    ItCtrl = new wxTextCtrl  (this, wxID_ANY, std::to_string(*numIt)       , wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    ///Dimensions sizer
    wxBoxSizer* dimensionsSizer = new wxBoxSizer(wxHORIZONTAL);
    dimensionsSizer->Add(SzXCtrl, 0, 0, 0);
    dimensionsSizer->Add(Sz2Stat, 0, 0, 0);
    dimensionsSizer->Add(SzYCtrl, 0, 0, 0);
    ///Text sizer -> grid
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(5,2,5,5);
    gridSizer->Add(  ReStat, 0, wxEXPAND, 0); gridSizer->Add(  ReCtrl, 0, 0, 0);
    gridSizer->Add(  ImStat, 0, wxEXPAND, 0); gridSizer->Add(  ImCtrl, 0, 0, 0);
    gridSizer->Add(StepStat, 0, wxEXPAND, 0); gridSizer->Add(StepCtrl, 0, 0, 0);
    gridSizer->Add( Sz1Stat, 0, wxEXPAND, 0); gridSizer->Add( dimensionsSizer, 0, 0, 0);
    gridSizer->Add(  ItStat, 0, wxEXPAND, 0); gridSizer->Add(  ItCtrl, 0, 0, 0);
    ///Static text
    wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Settings");
    staticBoxSizer->Add(gridSizer, 0, wxALL, 5);
    ///Buttons
        OKBtn = new wxButton(this, wxID_OK,         "OK");
    CancelBtn = new wxButton(this, wxID_CANCEL, "Cancel");
    ///Buttons sizer -> wxBoxSizer(wxHORIZONTAL)
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(OKBtn, 1, wxALL, 5); buttonSizer->Add(CancelBtn, 1, wxALL, 5);
    ///Global sizer
    wxBoxSizer* globalSizer = new wxBoxSizer(wxVERTICAL);
    globalSizer->Add(staticBoxSizer, 0, wxLEFT | wxTOP | wxRIGHT, 8);
    globalSizer->Add(   buttonSizer, 0,   wxALIGN_CENTER | wxALL, 5);

    globalSizer->SetSizeHints(this);
    this->SetSizer(globalSizer);
}
void HDPrintscreenDialog::OnOK(wxCommandEvent&){
    *center = FractalBitmap::ComplexNum(ato<FractalBitmap::complex_t>(ReCtrl->GetValue().ToStdString()),
                                        ato<FractalBitmap::complex_t>(ImCtrl->GetValue().ToStdString()));
    *step   =        ato<FractalBitmap::complex_t>(StepCtrl->GetValue().ToStdString());
    *sz     = wxSize(std::stoi(SzXCtrl->GetValue().ToStdString()),
                     std::stoi(SzYCtrl->GetValue().ToStdString()));
    *numIt  =        ato<FractalBitmap::iter_t>(ItCtrl->GetValue().ToStdString());
    this->EndModal(wxID_OK);
}
void HDPrintscreenDialog::OnCancel(wxCommandEvent&){
    this->EndModal(wxID_CANCEL);
}

wxBEGIN_EVENT_TABLE(HDPrintscreenDialog, wxDialog)
    EVT_BUTTON(wxID_OK,     HDPrintscreenDialog::OnOK    )
    EVT_BUTTON(wxID_CANCEL, HDPrintscreenDialog::OnCancel)
wxEND_EVENT_TABLE()
