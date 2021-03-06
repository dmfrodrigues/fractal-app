/**
 * file: FractalBitmap.h
 *
 * This header file provides the definition of the FractalBitmap class.
 * FractalBitmap provides an easy interface to allow calculations, plotting and
 * saving images of fractals.
 *
 * FractalBitmap inherits from wxBitmap, to inherit some useful functions, since
 * the FractalBitmap class is meant to be seen from the outside as a black-box
 * encapsulating a lot of heavy calculations, serving as a sort of an 'evolving
 * image'.
 *
 * FractalBitmap must not be used directly, but rather overloaded to correspond
 * to the user's needs.
 *
 * Currently, this class uses `long double` as fundamental floating type
 * (`complex_t`), and `std::complex<complex_t>` as fundamental complex type
 * (`ComplexNum`).
 * /!\ BEWARE THAT `std::complex` IS VERY SLOW if compiled with default options.
 * To make `std::complex` calculations decently fast, compile FractalBitmap.cpp
 * with the -ffast-math flag.
 */

#ifndef FRACTALBITMAP_H_INCLUDED
#define FRACTALBITMAP_H_INCLUDED

#include <complex>
#include <mutex>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif
#include <wx/wx.h>
#ifdef _MSC_VER
    #pragma warning(pop)
#endif
#include <wx/rawbmp.h>
#pragma GCC diagnostic pop

//#include "mc_float.h"

using namespace std;

class FractalBitmap: public wxBitmap {
public:
    // PUBLIC TYPEDEFS
    typedef long double complex_t;
    typedef unsigned long long iter_t;
    typedef std::complex<complex_t> ComplexNum;
private:
    /// Origin of the screen (upper-left corner) in complex coordinate units.
    ComplexNum origin;
    /// Step between consecutive pixels.
    complex_t  step;
public:
    std::mutex data;
    std::mutex next;
    std::mutex lowpriority;

    /**
     * @brief Construct a new Fractal Bitmap object without arguments.
     */
    FractalBitmap();

    /**
     * Creates fresh FractalBitmap, using the provided parameters.
     * @param   center      center of the fractal
     * @param   st          step (distance between two consecutive pixels)
     * @param   s           size of the fractal in pixels (width, height)
     */
    virtual void Create(ComplexNum center, complex_t step, wxSize sz);
    /**
     * FractalBitmap::Clone receives the same parameters as
     * FractalBitmap::Create. It creates a new object in the heap, runs Create on it,
     * and returns the pointer to the new object
     * @param   o           origin (or center) of the fractal
     * @param   st          step (distance between two consecutive pixels)
     * @param   s           size of the fractal in pixels (width, height)
     * @param   IsCenter    true if provided argument `o` is the center of the
                            fractal, false if `o` is the origin.
     */
    virtual FractalBitmap* Clone() const = 0;
    
    // CALCULATIONS ==================================================
    /**
     * Make calculations, and update pixels accordingly. This is the most
     * important function of the class.
     */
    virtual void Update() = 0;

    // GET FUNCTIONS =================================================
    /**
     * @brief Get the origin (top-left corner) complex coordinates.
     * 
     * @return ComplexNum Origin
     */
    virtual ComplexNum  GetOrigin()                   const final;
    /**
     * @brief Set the origin (top-left corner) complex coordinates.
     * 
     * @param orig New origin value
     */
    virtual void        SetOrigin(const ComplexNum &origin) final;
    /**
     * @brief Get the center of the screen in complex coordinates.
     * 
     * @return ComplexNum Center
     */
    virtual ComplexNum  GetCenter()                   const final;
    /**
     * @brief Set the center of the screen in complex coordinates.
     * 
     * @param cent New center value
     */
    virtual void        SetCenter(const ComplexNum &center) final;
    /**
     * @brief Get the step (in complex coordinate units) between consecutive pixels.
     * 
     * @return complex_t Step
     */
    virtual complex_t   GetStep()                     const final;
    /**
     * @brief Set the step between consecutive pixels.
     * 
     * @param stp New step value
     */
    virtual void        SetStep(const complex_t &step)      final;

    virtual iter_t GetNum()            const = 0;
    virtual complex_t   GetHorizontalSize() const final;
    virtual iter_t GetCyclesPerRun()   const = 0;

    // STATIC FUNCTIONS ==============================================
    /**
     * Get origin of fractal from the provided center in a fractal with step
     * `st` and size `s`, and vice-versa
     */
    static ComplexNum GetOriginFromCenter(ComplexNum cent, complex_t st, wxSize s);
    static ComplexNum GetCenterFromOrigin(ComplexNum orig, complex_t st, wxSize s);

};

#endif // FRACTALBITMAP_H_INCLUDED
