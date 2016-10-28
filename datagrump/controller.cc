#include <iostream>

#include "controller.hh"
#include "timestamp.hh"
#include <chrono>
#include <ratio>

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug )

  , the_window_size(50)
  , prev_time(0)
  , now_time(0)
  , rtt(100)
  , num_packets(0)
{ }

/* Get current window size, in datagrams */
unsigned int Controller::window_size( void )
{

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  // if (num_packets == 0 ) { }
  return the_window_size;
}

void Controller::update_window() {
  cerr << time(0) << "the_window_size: " << the_window_size << ", num_packets: " << num_packets << ", rtt: " << rtt << "\n";

  if (num_packets < 2 && the_window_size < 5 && rtt <= 2*TICK_MS) {
    // restart situation
    the_window_size = 30;
  }
  if (num_packets >= 1.1*the_window_size || rtt <= 2*TICK_MS) {
    the_window_size = the_window_size*0.7 + num_packets*0.3;
  } else if (num_packets <= 0.8*the_window_size || rtt >= 5*TICK_MS) {
    the_window_size = the_window_size*0.3 + num_packets*0.3;
  }

  num_packets = 0; 
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

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << endl;
  }

  // when ack is received, 
  uint64_t this_rtt = timestamp_ack_received - send_timestamp_acked;
  rtt = rtt*0.8 + 0.2*this_rtt;  
  
  if (ack_expected == sequence_number_acked) {
   // if (this_rtt < std::min(static_cast<unsigned int>(rtt*2), static_cast<unsigned int>(300))) {
      num_packets++;
   // }
  }

}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms( void )
{
  return TICK_MS; /* timeout of one second */
}
