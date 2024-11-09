#include "tween.h"

#include <vector>
#include "vec2d.h"

namespace Engine
{
    namespace Tween
    {

        struct TweenData
        {
            Vec2D *vec2d;
            int duration;
            int remaining_duration;
            int start_x;
            int start_y;
            int end_x;
            int end_y;
            void (*finish_callback)(void);
        };

        std::vector<TweenData> tween_data;

        void add_tween(Vec2D &vec2d, int duration, int x, int y, void (*cb)(void))
        {
            // Check if the draw object already exists in the tween list
            for (size_t i = 0; i < tween_data.size(); ++i)
            {
                if (tween_data[i].vec2d == &vec2d)
                {
                    tween_data.erase(tween_data.begin() + i);
                    break;
                }
            }

            TweenData data = {&vec2d, duration, duration, vec2d.x, vec2d.y, x, y, cb};
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
                    data.vec2d->x = data.end_x;
                    data.vec2d->y = data.end_y;
                    tween_data.erase(tween_data.begin() + i);
                    if (data.finish_callback != NULL)
                        data.finish_callback();
                }
                else
                {
                    double progress = (data.duration - data.remaining_duration) / static_cast<double>(data.duration);
                    int new_x = static_cast<int>(data.start_x + (data.end_x - data.start_x) * progress);
                    int new_y = static_cast<int>(data.start_y + (data.end_y - data.start_y) * progress);

                    data.vec2d->x = new_x;
                    data.vec2d->y = new_y;

                    tween_data[i].remaining_duration -= delta_time_milliseconds;
                    if (tween_data[i].remaining_duration <= 0)
                        tween_data[i].remaining_duration = 0;
                }
            }
        }

    } // namespace Tween
} // namespace Engine