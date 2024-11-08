#include "tween.h"

#include <vector>
#include "drawobj.h"

namespace Tween
{
    std::vector<void (*)(void)> tween_finish_callbacks;
    std::vector<DrawObject *> tween_drawobjects;
    std::vector<int> tween_durations;
    std::vector<int> tween_remaining_durations;
    std::vector<int> tween_start_x_positions;
    std::vector<int> tween_start_y_positions;
    std::vector<int> tween_end_x_positions;
    std::vector<int> tween_end_y_positions;

    void add_tween(DrawObject &drawObject, int duration, int x, int y, void (*cb)(void))
    {
        // Check if the drawObject already exists in the tween list
        for (size_t i = 0; i < tween_drawobjects.size(); ++i)
        {
            if (tween_drawobjects[i] == &drawObject)
            {
                tween_drawobjects.erase(tween_drawobjects.begin() + i);
                tween_durations.erase(tween_durations.begin() + i);
                tween_remaining_durations.erase(tween_remaining_durations.begin() + i);
                tween_start_x_positions.erase(tween_start_x_positions.begin() + i);
                tween_start_y_positions.erase(tween_start_y_positions.begin() + i);
                tween_end_x_positions.erase(tween_end_x_positions.begin() + i);
                tween_end_y_positions.erase(tween_end_y_positions.begin() + i);
                tween_finish_callbacks.erase(tween_finish_callbacks.begin() + i);
                break;
            }
        }

        tween_finish_callbacks.push_back(cb);
        tween_drawobjects.push_back(&drawObject);
        tween_durations.push_back(duration);
        tween_remaining_durations.push_back(duration);
        tween_start_x_positions.push_back(drawObject.x);
        tween_start_y_positions.push_back(drawObject.y);
        tween_end_x_positions.push_back(x);
        tween_end_y_positions.push_back(y);
    }

    void handle_tweens(double delta_time)
    {
        int delta_time_milliseconds = delta_time * 1000;
        for (int i = tween_drawobjects.size() - 1; i >= 0; i--)
        {
            DrawObject *drawObject = tween_drawobjects[i];
            void (*finish_callback)(void) = tween_finish_callbacks[i];
            int duration = tween_durations[i];
            int remaining_duration = tween_remaining_durations[i];
            int start_x = tween_start_x_positions[i];
            int start_y = tween_start_y_positions[i];
            int end_x = tween_end_x_positions[i];
            int end_y = tween_end_y_positions[i];

            if (duration <= 0)
            {
                drawObject->set_position(end_x, end_y);
                tween_drawobjects.erase(tween_drawobjects.begin() + i);
                tween_durations.erase(tween_durations.begin() + i);
                tween_remaining_durations.erase(tween_remaining_durations.begin() + i);
                tween_start_x_positions.erase(tween_start_x_positions.begin() + i);
                tween_start_y_positions.erase(tween_start_y_positions.begin() + i);
                tween_end_x_positions.erase(tween_end_x_positions.begin() + i);
                tween_end_y_positions.erase(tween_end_y_positions.begin() + i);
                tween_finish_callbacks.erase(tween_finish_callbacks.begin() + i);
                if (finish_callback != NULL)
                    finish_callback();
            }
            else
            {
                double progress = (duration - remaining_duration) / static_cast<double>(duration);
                int new_x = static_cast<int>(start_x + (end_x - start_x) * progress);
                int new_y = static_cast<int>(start_y + (end_y - start_y) * progress);

                drawObject->set_position(new_x, new_y);

                tween_remaining_durations[i] -= delta_time_milliseconds;
                if (tween_remaining_durations[i] <= 0)
                    tween_remaining_durations[i] = 0;
            }
        }
    }

} // namespace Tween