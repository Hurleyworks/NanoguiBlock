/*
    nanogui/graph.h -- Simple graph widget for showing a function plot

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

NAMESPACE_BEGIN (nanogui)

class  Graph : public Widget
{

   public:
	  typedef std::vector<float> VectorXf;

   public:
      Graph (Widget * parent, const std::string & caption = "Untitled");

      const std::string & caption() const
      {
         return mCaption;
      }
      void setCaption (const std::string & caption)
      {
         mCaption = caption;
      }

      const std::string & header() const
      {
         return mHeader;
      }
      void setHeader (const std::string & header)
      {
         mHeader = header;
      }

      const std::string & footer() const
      {
         return mFooter;
      }
      void setFooter (const std::string & footer)
      {
         mFooter = footer;
      }

      const Colour & backgroundColor() const
      {
         return mBackgroundColor;
      }
      void setBackgroundColor (const Colour & backgroundColor)
      {
         mBackgroundColor = backgroundColor;
      }

      const Colour & foregroundColor() const
      {
         return mForegroundColor;
      }
      void setForegroundColor (const Colour & foregroundColor)
      {
         mForegroundColor = foregroundColor;
      }

      const Colour & textColor() const
      {
         return mTextColor;
      }
      void setTextColor (const Colour & textColor)
      {
         mTextColor = textColor;
      }

      const VectorXf & values() const
      {
         return mValues;
      }
      VectorXf & values()
      {
         return mValues;
      }
      void setValues (const VectorXf & values)
      {
         mValues = values;
      }

      virtual ivec2 preferredSize (NVGcontext * ctx) const;
      virtual void draw (NVGcontext * ctx);
   protected:
      std::string mCaption, mHeader, mFooter;
      Colour mBackgroundColor, mForegroundColor, mTextColor;
      VectorXf mValues;
};

NAMESPACE_END (nanogui)
