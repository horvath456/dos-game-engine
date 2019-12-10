#include "animobj.h"

#include "screen.h"
#include "drawobj.h"

AnimationObject::AnimationObject(unsigned int numframes, const char **filenames)
{
    frame = 0;
    num_frames = numframes;
    draw_objects = new DrawObject *[num_frames];

    for (unsigned int i = 0; i < num_frames; i++)
    {
        draw_objects[i] = new DrawObject(filenames[i]);
    }
}

AnimationObject::~AnimationObject()
{
    for (unsigned int i = 0; i < num_frames; i++)
    {
        delete draw_objects[i];
    }
    delete[] draw_objects;
}

void AnimationObject::draw(Screen &screen, int x, int y, int transparent)
{
    draw_objects[frame]->draw(screen, x, y, transparent);
}