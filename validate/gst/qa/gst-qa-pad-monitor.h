/* GStreamer
 * Copyright (C) 2013 Thiago Santos <thiago.sousa.santos@collabora.com>
 *
 * gst-qa-pad-monitor.h - QA PadMonitor class
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_QA_PAD_MONITOR_H__
#define __GST_QA_PAD_MONITOR_H__

#include <glib-object.h>
#include <gst/gst.h>

#include "gst-qa-monitor.h"

G_BEGIN_DECLS

/* forward declaratin */
typedef struct _GstQaElementMonitor GstQaElementMonitor;

#define GST_TYPE_QA_PAD_MONITOR			(gst_qa_pad_monitor_get_type ())
#define GST_IS_QA_PAD_MONITOR(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_QA_PAD_MONITOR))
#define GST_IS_QA_PAD_MONITOR_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_QA_PAD_MONITOR))
#define GST_QA_PAD_MONITOR_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_QA_PAD_MONITOR, GstQaPadMonitorClass))
#define GST_QA_PAD_MONITOR(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_QA_PAD_MONITOR, GstQaPadMonitor))
#define GST_QA_PAD_MONITOR_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_QA_PAD_MONITOR, GstQaPadMonitorClass))
#define GST_QA_PAD_MONITOR_CAST(obj)            ((GstQaPadMonitor*)(obj))
#define GST_QA_PAD_MONITOR_CLASS_CAST(klass)    ((GstQaPadMonitorClass*)(klass))

#define GST_QA_PAD_MONITOR_GET_PAD(m) (GST_PAD_CAST (GST_QA_MONITOR_GET_OBJECT (m)))

typedef struct _GstQaPadMonitor GstQaPadMonitor;
typedef struct _GstQaPadMonitorClass GstQaPadMonitorClass;


/**
 * GstQaPadMonitor:
 *
 * GStreamer QA PadMonitor class.
 *
 * Class that wraps a #GstPad for QA checks
 */
struct _GstQaPadMonitor {
  GstQaMonitor 	 parent;

  GstQaElementMonitor *element_monitor;

  gboolean       setup;
  GstPad        *pad;

  GstPadBufferAllocFunction bufferalloc_func;
  GstPadChainFunction chain_func;
  GstPadEventFunction event_func;
  GstPadGetRangeFunction getrange_func;
  GstPadQueryFunction query_func;
  GstPadSetCapsFunction setcaps_func;
  GstPadGetCapsFunction getcaps_func;

  gulong buffer_probe_id;
  gulong event_probe_id;

  /*< private >*/
  gboolean first_buffer;

  gboolean has_segment;
  gboolean is_eos;

  gboolean pending_flush_stop;
  guint32 pending_flush_stop_seqnum;
  guint32 pending_flush_start_seqnum;
  guint32 pending_newsegment_seqnum;

  GstEvent *expected_segment;
  GPtrArray *serialized_events;

  GstStructure *pending_setcaps_fields;

  /* tracked data */
  GstSegment segment;
  GstClockTime current_timestamp;
  GstClockTime current_duration;

  GstFlowReturn last_flow_return;

  /* Stores the timestamp range of data that has flown through
   * this pad by using TIMESTAMP and TIMESTAMP+DURATION from
   * incomming buffers. Every time a buffer is pushed, this range
   * is extended.
   *
   * When a buffer is pushed, the timestamp range is checked against
   * the outgoing timestamp to check it is in the received boundaries.
   */
  GstClockTime timestamp_range_start;
  GstClockTime timestamp_range_end;
};

/**
 * GstQaPadMonitorClass:
 * @parent_class: parent
 *
 * GStreamer QA PadMonitor object class.
 */
struct _GstQaPadMonitorClass {
  GstQaMonitorClass	parent_class;
};

/* normal GObject stuff */
GType		gst_qa_pad_monitor_get_type		(void);

GstQaPadMonitor *   gst_qa_pad_monitor_new      (GstPad * pad, GstQaRunner * runner, GstQaElementMonitor *element_monitor);

G_END_DECLS

#endif /* __GST_QA_PAD_MONITOR_H__ */

