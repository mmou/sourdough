#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <cstdint>

#define DUP_ACK_COUNT  1
#define TICK_MS 1000


/* Congestion controller interface */

class Controller
{
private:
  bool debug_; /* Enables debugging output */

  /* Add member variables here */ 
  unsigned int the_window_size;
  time_t prev_time;
  time_t now_time;
  uint64_t rtt;
  uint64_t num_packets;


public:
  /* Public interface for the congestion controller */
  /* You can change these if you prefer, but will need to change
     the call site as well (in sender.cc) */

  /* Default constructor */
  Controller( const bool debug);

  /* Get current window size, in datagrams */
  unsigned int window_size( void );

  /* Update window size */
  void update_window(auto diff_time);

  /* A datagram was sent */
  void datagram_was_sent( const uint64_t sequence_number,
			  const uint64_t send_timestamp );

  /* An ack was received */
  void ack_received( const uint64_t sequence_number_acked,
		     const uint64_t send_timestamp_acked,
		     //const uint64_t recv_timestamp_acked,
		     const uint64_t timestamp_ack_received,
         const uint64_t ack_expected);


  /* How long to wait (in milliseconds) if there are no acks
     before sending one more datagram */
  unsigned int timeout_ms( void );
};

#endif
