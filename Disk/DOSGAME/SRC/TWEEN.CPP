#include "tween.h"

#include <vector>
#include "drawobj.h"

namespace Tween
{

    struct TweenData
    {
        DrawObject *draw_object;
        int duration;
        int remaining_duration;
        int start_x;
        int start_y;
        int end_x;
        int end_y;
        void (*finish_callback)(void);
    };

    std::vector<TweenData> tween_data;

    void add_tween(DrawObject &draw_object, int duration, int x, int y, void (*cb)(void))
    {
        // Check if the draw object already exists in the tween list
        for (size_t i = 0; i < tween_data.size(); ++i)
        {
            if (tween_data[i].draw_object == &draw_object)
            {
                tween_data.erase(tween_data.begin() + i);
                break;
            }
        }

        TweenData data = {&draw_object, duration, duration, draw_object.x, draw_object.y, x, y, cb};
        tween_data.push_back(data);
    }

    void handle_tweens(double delta_time)
    {
        int delta_time_milliseconds = delta_time * 1000;
        for (int i = tween_data.size() - 1; i >= 0; i--)
        {
            TweenData data = tween_data[i];

            if (data.duration <= 0)
            {
                data.draw_object->set_position(data.end_x, data.end_y);
                tween_data.erase(tween_data.begin() + i);
                if (data.finish_callback != NULL)
                    data.finish_callback();
            }
            else
            {
                double progress = (data.duration - data.remaining_duration) / static_cast<double>(data.duration);
                int new_x = static_cast<int>(data.start_x + (data.end_x - data.start_x) * progress);
                int new_y = static_cast<int>(data.start_y + (data.end_y - data.start_y) * progress);

                data.draw_object->set_position(new_x, new_y);

                tween_data[i].remaining_duration -= delta_time_milliseconds;
                if (tween_data[i].remaining_duration <= 0)
                    tween_data[i].remaining_duration = 0;
            }
        }
    }

} // namespace Tween