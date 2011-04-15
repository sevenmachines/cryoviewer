//#include "klGLEW.h"
#include "osgGA/TrackballManipulator"
#include "osgGA/FlightManipulator"
#include "osgGA/DriveManipulator"
#include "osgUtil/DisplayRequirementsVisitor"
//
#include "OSGDrawingArea.h"

//----------------------------OSGDrawingArea--------------------------------------------------------------------------------
OSGDrawingArea::OSGDrawingArea ()
{

    mMX = 0.;
    mMY = 0.;
    mButtonMask = 0;

    add_events (Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK |
        Gdk::BUTTON_MOTION_MASK | Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON2_MOTION_MASK |
        Gdk::BUTTON3_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
        Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK | Gdk::ENTER_NOTIFY_MASK |
        Gdk::LEAVE_NOTIFY_MASK);

    //
    //Configure OpenGL-capable visual.
    Glib::RefPtr<Gdk::GL::Config> lGlConfig;

    //
    //Try double-buffered visual first.
    lGlConfig = Gdk::GL::Config::create (Gdk::GL::MODE_RGBA | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_DOUBLE);
    if (!lGlConfig)
    {
        std::cerr << "*** Cannot find the double-buffered visual.\n"
                    << "*** Trying single-buffered visual." << std::endl;
        //
        //Try single-buffered visual.
        lGlConfig = Gdk::GL::Config::create (Gdk::GL::MODE_RGBA | Gdk::GL::MODE_DEPTH);
        if (!lGlConfig)
        {
            std::cerr << "*** Cannot find any OpenGL-capable visual." << std::endl;
            std::exit (1);//????????????AAAAAAAAAAAAAAAAAARRRGH!
        }//if
    }//if

    set_gl_capability (lGlConfig);//Add OpenGL-capability to the widget.

    //
    //This is fundamental, because the on_configure_event is not called on children
    //widgets. Wtf. So this signal hooking is needed.
    this->signal_size_allocate ().connect
        (sigc::mem_fun (*this, &OSGDrawingArea::onResizeInternal));
}

void OSGDrawingArea::on_realize ()
{
#ifdef DEBUG
    std::cout <<"void OSGDrawingArea::on_realize ()<<"<< std::endl;
#endif
    Gtk::DrawingArea::on_realize ();

    //
    // Init glew.
    /*klGLEW::instance ()->init ();
    if (klGLEW::instance ()->isInitialized ())
        KDBG_OUT ("", "GLEW init() succesfully");
    else
        KDBG_OUT ("", "GLEW init() failed!");*/


    if (!beginGL ())
        return;

    onRealize ();

    endGL ();
#ifdef DEBUG
    std::cout <<"void OSGDrawingArea::on_realize ()>>"<< std::endl;
#endif
}

bool OSGDrawingArea::on_motion_notify_event (GdkEventMotion* pEvent)
{
#ifdef DEBUG
    //??std::cout <<  "OSGDrawingArea::onMouseMoveInternal()<<" << std::endl;
#endif

    //
    //
    // Since we are using the POINTER_MOTION_HINT_MASK mask we need to call this on any event.
    if (pEvent->is_hint)
    {
        int lX, lY;
        Gdk::ModifierType lState;
        get_window ()->get_pointer (lX, lY, lState);
        pEvent->x = lX;
        pEvent->y = lY;//?????????get_height () - lY - 1;
        pEvent->state = lState;
    }//if

    bool lRetVal = onMouseMove (pEvent);

#ifdef DEBUG
    //??std::cout <<  "OSGDrawingArea::onMouseMoveInternal()>>" << std::endl;
#endif//
    return lRetVal;
}

bool OSGDrawingArea::on_button_press_event (GdkEventButton* pEvent)
{
    return onButtonPress (pEvent);
}

bool OSGDrawingArea::on_button_release_event (GdkEventButton* pEvent)
{
    return onButtonRelease (pEvent);
}


void OSGDrawingArea::on_size_allocate (Gtk::Allocation& pAllocation)
{
#ifdef DEBUG
    //??std::cout <<"void OSGDrawingArea::on_size_allocate (Gtk::Allocation& pAllocation)<<"<< std::endl;
#endif
    set_allocation (pAllocation);
    if (is_realized ())
    {
        get_window ()->move_resize (pAllocation.get_x (), pAllocation.get_y (),
            pAllocation.get_width (), pAllocation.get_height ());
    }//if
#ifdef DEBUG
    //??std::cout <<"void OSGDrawingArea::on_size_allocate (Gtk::Allocation& pAllocation)>>"<< std::endl;
#endif
}


bool OSGDrawingArea::on_configure_event (GdkEventConfigure* pEvent)
{
#ifdef DEBUG
    //??std::cout <<"bool OSGDrawingArea::on_configure_event (GdkEventConfigure* pEvent)<<"<< std::endl;
#endif

    bool lRetVal = Gtk::DrawingArea::on_configure_event (pEvent);

    if (!beginGL ())
        return lRetVal;

    onConfigure (pEvent);

    endGL ();

#ifdef DEBUG
    //??std::cout <<"bool OSGDrawingArea::on_configure_event (GdkEventConfigure* pEvent)>>"<< std::endl;
#endif
    return lRetVal;
}

void OSGDrawingArea::onResizeInternal (Gtk::Allocation& pAllocation)
{
#ifdef DEBUG
    //??std::cout <<"void OSGDrawingArea::onResizeInternal (Gtk::Allocation& pAllocation)<<"<< std::endl;
#endif // DEBUG

    if (!is_realized ())
        return;

    if (!beginGL ())
        return;

    onResize (pAllocation);

    endGL ();

#ifdef DEBUG
    //??std::cout <<"void OSGDrawingArea::onResizeInternal (Gtk::Allocation& pAllocation)>>"<< std::endl;
#endif // DEBUG
}

bool OSGDrawingArea::on_expose_event (GdkEventExpose* pEvent)
{
#ifdef DEBUG
    //??std::cout <<"bool OSGDrawingArea::on_expose_event (GdkEventExpose* pEvent)<<"<< std::endl;
#endif // DEBUG

    bool lRetVal = Gtk::DrawingArea::on_expose_event (pEvent);

    if (!beginGL())
        return lRetVal;

    onExpose (pEvent);

    swapBuffers ();

    endGL ();

#ifdef DEBUG
    //??std::cout <<"bool OSGDrawingArea::on_expose_event (GdkEventExpose* pEvent)>>"<< std::endl;
#endif // DEBUG

    return lRetVal;
}

void OSGDrawingArea::updateView (bool pFast)
{
#ifdef DEBUG
    //??std::cout << "(OSGDrawingArea*)" << this << "->updateView (" << pFast << ")" << std::endl;
#endif // DEBUG

    invalidate ();//Invalidate whole window.

    if (pFast)
        update ();//Update window synchronously (fast).
}

void OSGDrawingArea::invalidate ()
{
    Gdk::Rectangle lRect = get_allocation ();
    get_window ()->invalidate_rect (Gdk::Rectangle (0, 0, lRect.get_width (), lRect.get_height ()),
        false);
}

void OSGDrawingArea::update ()
{
    //Normally GDK calls gdk_window_process_all_updates() on your behalf, so there's no need to
    //call this function unless you want to force expose events to be delivered immediately and
    //synchronously (vs. the usual case, where GDK delivers them in an idle handler).
    get_window ()->process_updates (false);
}

//--------------------------------------------------------------------------------------------------------
