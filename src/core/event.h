#ifndef _I8080_EVENT_H
#define _I8080_EVENT_H

/* Information about an event */
typedef struct event_t {
  DWORD tstates;
  int type;
} event_t;

/* The various types of event which can occur */
enum event_types {
  EVENT_TYPE_NULL,
  EVENT_TYPE_INTERRUPT,
  EVENT_TYPE_LINE,
  EVENT_TYPE_EDGE,
};

/* Ѕольшое значение обозначающее что нет событий */
extern const DWORD event_no_events;

/*  огда должно произойти следующее событие? */
extern DWORD event_next_event;

/* »нициализаци€ списка событий */
int event_init(void);

/* Add an event at the correct place in the event list */
int event_add(DWORD event_time, int type);

/* Do all events which have passed */
int event_do_events(void);

/* Called on interrupt to reduce T-state count of all entries */
int event_interrupt( DWORD tstates_per_frame );

/* Remove all events of a specific type from the stack */
int event_remove_type( int type );

/* Clear the event stack */
int event_reset(void);

/* Called on exit to clean up */
int event_end(void);

#endif				/* #ifndef FUSE_EVENT_H */
