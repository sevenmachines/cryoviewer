#ifndef __OSGDRAWINGAREA_H__
#define __OSGDRAWINGAREA_H__

#ifdef ENABLE_OSG

#include <iostream>
#include <gtkmm.h>
#include <gtkglmm.h>
#include <osg/Timer>
#include <osgUtil/SceneView>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventAdapter>
#include <osgGA/KeySwitchMatrixManipulator>

using namespace std;


class OSGDrawingArea : public Gtk::DrawingArea, public Gtk::GL::Widget<OSGDrawingArea>
{


//--------------------event related-------------------------------------
protected:
    gdouble mMX;
    gdouble mMY;
    guint mButtonMask;

    bool on_motion_notify_event (GdkEventMotion* pEvent);
    bool on_button_release_event (GdkEventButton* pEvent);
    bool on_button_press_event (GdkEventButton* pEvent);
public:
    virtual bool onButtonRelease (GdkEventButton* pEvent)
    {
        mButtonMask &= ~(1 << (pEvent->button - 1));
        return true;
    }

    virtual bool onButtonPress (GdkEventButton* pEvent)
    {
        mButtonMask |= 1 << (pEvent->button - 1);
        return true;
    }

    virtual bool onMouseMove (GdkEventMotion* pEvent)
    {
        mMX = pEvent->x;
        mMY = pEvent->y;
        return true;
    }
//--------------------event related-------------------------------------

public:
    OSGDrawingArea ();

    void on_realize ();
    virtual void onRealize () = 0;

    bool on_configure_event (GdkEventConfigure* pEvent);
    virtual bool onConfigure (GdkEventConfigure* pEvent) = 0;

protected:
    void onResizeInternal (Gtk::Allocation& pAllocation);
public:
    /// Update the viewport dimensions, incase the window has been resized.
    virtual void onResize (Gtk::Allocation& pAllocation) = 0;

    void on_size_allocate (Gtk::Allocation& pAllocation);

    bool on_expose_event (GdkEventExpose* pEvent);
    virtual bool onExpose (GdkEventExpose* pEvent) = 0;

    inline bool beginGL ()
    {
        if (!get_gl_window ()->gl_begin (get_gl_context ()))
        {
            std::cout <<"get_gl_window ()->gl_begin (get_gl_context ())) failed!" <<std::endl;
            return false;
        }//if
        return true;
    }

    inline void endGL ()
    {
        get_gl_window ()->gl_end ();
    }

    ///Swap buffers if double buffered in use, otherwise just flushes OGL.
    inline void swapBuffers ()
    {
        if (get_gl_window ()->is_double_buffered ())
            get_gl_window ()->swap_buffers ();
        else
            glFlush ();
    }

    void updateView (bool pFast);

    void invalidate ();

    void update ();

};//class

#endif // __OSGDRAWINGAREA_H__

#endif
