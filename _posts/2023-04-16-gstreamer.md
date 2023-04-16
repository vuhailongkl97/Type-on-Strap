---
published: true
---
# Gstreamer elements

gstream have several type of element : source, sink, transform, filter, mixer

sources : videtestsrc, rtsp
sink : autovideosink, fakesink
transform: videoconvert, codec
filter : set bitrate, scale , crop, overlay

## Basic concepts
**Pipeline**: A pipeline is a sequence of elements connected together, which together define a media processing flow.

**Element**: An element is a unit of processing in GStreamer, such as a video source or a video filter. Elements have pads that connect to other elements in the pipeline.

**Pad**: A pad is a logical endpoint of an element that connects to other elements in the pipeline, enabling data to be sent and received between elements.

**Bin**: A bin is a container for elements, used to organize and encapsulate elements within a pipeline.

**Plugin**: A plugin is a loadable module that provides one or more GStreamer elements, such as a video decoder or an audio encoder.

**Caps**: Caps is short for "capabilities" and describes the media format and properties of data flowing through a pipeline.

**Pipeline** state: The pipeline state describes whether a pipeline is running, paused, or stopped.

**Bus**: The bus is a message passing mechanism used to notify applications about events in the pipeline, such as errors or state changes.


## Create a transform or filter element

It can be done by 
+ create a probe function with `gst_pad_add_probe` by register probe with the element pad ( called when data come to the pad )

+ create transform element and register transform_ip function [code](https://github.com/vuhailongkl97/gstreamer-gst-template/blob/8e74bad63c729e032041da3296cd090a720a3946/gst-plugin/src/gstmyfilter.c#L140) (called when data flow come in the plugin)

+ create chain callback ( is called when a new sample come in)

## Notes

need to understand what type of image is processing (RGBA, RGB, YUVI,..)
```
GstPad* pad = gst_element_get_static_pad(videotestsrc, "src");
GstCaps* caps = gst_pad_get_current_caps(pad);

if (caps) {
  GstVideoInfo info;
  if (gst_video_info_from_caps(&info, caps)) {
    g_print("Video format: %dx%d, framerate=%d/%d, format=%s\n",
            GST_VIDEO_INFO_WIDTH(&info), GST_VIDEO_INFO_HEIGHT(&info),
            GST_VIDEO_INFO_FPS_N(&info), GST_VIDEO_INFO_FPS_D(&info),
            gst_video_format_to_string(GST_VIDEO_INFO_FORMAT(&info)));
  }

  gst_caps_unref(caps);
}

gst_object_unref(pad);
```
