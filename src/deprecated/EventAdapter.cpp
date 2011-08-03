//
#include "EventAdapter.h"


EventAdapter::EventAdapter ()
{
    mEventType = NONE;           // adaptor does not encapsulate any events.
    mKey = -1;                   // set to 'invalid' key value.
    mButton = -1;                // set to 'invalid' button value.
    mMX = -1;                    // set to 'invalid' position value.
    mMY = -1;                    // set to 'invalid' position value.
    mButtonMask = 0;             // default to no mouse buttons being pressed.
    mTime = 0.0f;                // default to no time has been set.
}

void EventAdapter::setButtonMask(unsigned int buttonMask)
{
}


void EventAdapter::adaptMousePress (float time, guint buttonMask, gdouble x, gdouble y)
{
    mEventType = PUSH;
    mTime = time;
    mMX = (int)x;
    mMY = (int)y;
    mButtonMask=buttonMask;
}

void EventAdapter::adaptMouseRelease (float time, guint buttonMask, gdouble x, gdouble y)
{
    mEventType = RELEASE;
    mTime = time;
    mMX = (int)x;
    mMY = (int)y;
    mButtonMask=buttonMask;
}

void EventAdapter::adaptMouseMove (float time, guint buttonMask, gdouble x, gdouble y)
{
    if (buttonMask)
    {
        mEventType = DRAG;
    }
    else
    {
        mEventType = MOVE;
    }

    mTime = time;
    mMX = (int)x;
    mMY = (int)y;
    mButtonMask = buttonMask;
}

void EventAdapter::adaptReshape (float time, double width, double height)
{
}

void EventAdapter::adaptResize (float time, int Xmin, int Ymin, int Xmax, int Ymax)
{
    mXMin=Xmin;
    mXMax=Xmax;
    mYMin=Ymin;
    mYMax=Ymax;
    mEventType = RESIZE;
    mTime = time;
}

void EventAdapter::adaptFrame (float time)
{
    mEventType = FRAME;
    mTime = time;
}


