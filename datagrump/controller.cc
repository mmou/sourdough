#include <iostream>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug )

  , the_window_size(50)

  , prev_time(0)
  , now_time(0)
  , rtt(0)
  , num_packets(0)
{ }

/* Get current window size, in datagrams */
unsigned int Controller::window_size( void )
{

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  return the_window_size;
}

void Controller::update_window(time_t this_prev_time, time_t this_now_time) {
  if (now_time == this_prev_time) {
  
    prev_time = this_prev_time;
    now_time = this_now_time;
    num_packets = num_packets;

  // TODO: update window size based on ...!!!
  the_window_size = the_window_size*0.5 + num_packets/(now_time-prev_time)*rtt; 
  }
}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp )
                                    /* in milliseconds */
{

  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << endl;
  }

  // TODO: update model of available bandwidth, rtt

}

/* An ack was received */
void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       //const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received,
                               /* when the ack was received (by sender) */
             const uint64_t ack_expected)
{

/*
  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;

    cerr << timestamp_ack_received << ": " << timestamp_ack_received - send_timestamp_acked << "; " << sequence_number_acked << ", " << ack_expected << "\n";

  }
*/

  rtt = rtt*0.8 + 0.2*(timestamp_ack_received - send_timestamp_acked);
  
  if (ack_expected == sequence_number_acked) {
    num_packets++;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms( void )
{
  return TICK_MS; /* timeout of one second */
}
