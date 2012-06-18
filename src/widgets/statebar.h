/* ASEPRITE
 * Copyright (C) 2001-2012  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef WIDGETS_STATEBAR_H_INCLUDED
#define WIDGETS_STATEBAR_H_INCLUDED

#include <string>
#include <vector>

#include "app/color.h"
#include "base/compiler_specific.h"
#include "listeners.h"
#include "ui/base.h"
#include "ui/link_label.h"
#include "ui/widget.h"

class ColorButton;
class StatusBar;

namespace ui {
  class Box;
  class Button;
  class Entry;
  class Frame;
  class Slider;
}

namespace tools {
  class Tool;
}

class Progress
{
  friend class StatusBar;

public:
  ~Progress();
  void setPos(float pos);
  float getPos() const;

private:
  Progress();
  Progress(StatusBar* statusbar);
  StatusBar* m_statusbar;
  float m_pos;
};

class StatusBarListener
{
public:
  virtual ~StatusBarListener() { }
  virtual void dispose() = 0;
  virtual void onChangeTransparentColor(const Color& color) = 0;
};

typedef Listeners<StatusBarListener> StatusBarListeners;

class StatusBar : public ui::Widget
{
public:
  StatusBar();
  ~StatusBar();

  void addListener(StatusBarListener* listener);
  void removeListener(StatusBarListener* listener);

  void clearText();

  bool setStatusText(int msecs, const char *format, ...);
  void showTip(int msecs, const char *format, ...);
  void showColor(int msecs, const char* text, const Color& color, int alpha);
  void showTool(int msecs, tools::Tool* tool);

  void showMovePixelsOptions();
  void hideMovePixelsOptions();

  Color getTransparentColor();

  // Methods to add and remove progress bars
  Progress* addProgress();
  void removeProgress(Progress* progress);

  // Method to show notifications (each notification can contain a link).
  void showNotification(const char* text, const char* link);

protected:
  bool onProcessMessage(ui::Message* msg) OVERRIDE;

private:
  void onCurrentToolChange();
  void onTransparentColorChange();
  void updateFromLayer();
  void updateCurrentFrame();
  void newFrame();

  enum State { SHOW_TEXT, SHOW_COLOR, SHOW_TOOL };

  typedef std::vector<Progress*> ProgressList;

  int m_timeout;
  State m_state;

  // Showing a tool
  tools::Tool* m_tool;

  // Showing a color
  Color m_color;
  int m_alpha;

  // Progress bar
  ProgressList m_progress;

  // Box of main commands
  ui::Widget* m_commandsBox;
  ui::Slider* m_slider;             // Opacity slider
  ui::Entry* m_currentFrame;        // Current frame and go to frame entry
  ui::Button* m_newFrame;           // Button to create a new frame
  ui::Button* m_b_first;            // Go to first frame
  ui::Button* m_b_prev;             // Go to previous frame
  ui::Button* m_b_play;             // Play animation
  ui::Button* m_b_next;             // Go to next frame
  ui::Button* m_b_last;             // Go to last frame

  // Box of notifications.
  ui::Widget* m_notificationsBox;
  ui::LinkLabel* m_linkLabel;

  // Box with move-pixels commands (when the user drag-and-drop selected pixels using the editor)
  ui::Box* m_movePixelsBox;
  ui::Widget* m_transparentLabel;
  ColorButton* m_transparentColor;

  // Tip window
  class CustomizedTipWindow;
  CustomizedTipWindow* m_tipwindow;

  int m_hot_layer;

  StatusBarListeners m_listeners;
};

#endif
