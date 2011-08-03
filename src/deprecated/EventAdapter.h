#ifndef __EVENTADAPTER_H__
#define __EVENTADAPTER_H__

#include <gtkmm.h>
#include <osgGA/GUIEventAdapter>

/// Class for adapting GLUT events so that they can be used as input to osgUtil::CameraManipulators.
struct EventAdapter : public osgGA::GUIEventAdapter
{
    EventAdapter ();
    virtual ~EventAdapter ()
    {
    }

    /// Get the EventType of the GUI event.
    virtual EventType getEventType() const
    {
        return mEventType;
    }

    /// Key pressed, return -1 if inappropriate for this event.
    virtual int getKey() const
    {
        return mKey;
    }

    /// Button pressed/released, return -1 if inappropriate for this event.
    virtual int getButton() const { return mButton; }

    /// Window minimum x.
    virtual float getXmin() const  { return mXMin; }

    /// Window maximum x.
    virtual float getXmax() const  { return mXMax; }

    /// Window minimum y.
    virtual float getYmin() const { return mYMin; }

    /// Window maximum y.
    virtual float getYmax() const { return mYMax; }

    /// Current mouse x position.
    virtual float getX() const { return mMX; }

    /// Current mouse y position.
    virtual float getY() const { return mMY; }

    /// Current mouse button state.
    virtual unsigned int getButtonMask() const { return mButtonMask; }

    /// Unsupported!
    virtual unsigned int getModKeyMask() const { return 0; }

    /// Time in seconds of event.
    virtual double time () const { return mTime; }

    /// Static method for setting button state.*
    static void setButtonMask(unsigned int mButtonMask);

    void adaptMousePress (float time, guint buttonMask, gdouble x, gdouble y);
    void adaptMouseRelease (float time, guint buttonMask, gdouble x, gdouble y);
    void adaptMouseMove (float time, guint buttonMask, gdouble x, gdouble y);
    void adaptReshape (float time, double width, double height);

    /// Method for adapting resize events.
    void adaptResize (float t, int Xmin, int Ymin, int Xmax, int Ymax);

    /// Method for adapting frame events, i.e. idle/display callback.
    void adaptFrame (float t);

protected:
    EventType mEventType;
    int mKey;
    int mButton;
    int mXMin,mXMax;
    int mYMin, mYMax;
    int mMX;
    int mMY;
    unsigned int mButtonMask;
    float mTime;
};

#endif // __EVENTADAPTER_H__
