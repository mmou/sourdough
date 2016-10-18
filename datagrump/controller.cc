#include <iostream>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug )
{
  last_ack_recved_ = 0;
  dup_ack_count_ = 0;
  window_size = 50;
}

/* Get current window size, in datagrams */
unsigned int Controller::window_size( void )
{
  /* Default: fixed window size of 100 outstanding datagrams */
  //unsigned int the_window_size = 50;

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms() << " window size is " << window_size << endl;
  }

  return window_size;
}


void Controller:update_window(uint64_t sequence_number_acked) {

    if ( debug_ ) {
      cerr << "UPDATE_WINDOW last_ack_recved_: " << last_ack_recved_ << ","
      << "sequence_number_acked" << sequence_number_acked;
    }

    if (sequence_number_acked == last_ack_recved_) {
      dup_ack_count_ ++;
      if (dup_ack_count_ >= DUP_ACK_COUNT) {
        window_size /= 2;
        dup_ack_count_ = 0;
      }
    } else if (sequence_number_acked > last_ack_recved_) {
      window_size ++;
      last_ack_recved_ = sequence_number_acked;
      dup_ack_count_ = 0;
    } 

}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp )
                                    /* in milliseconds */
{
  /* Default: take no action */

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
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  /* Default: take no action */

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }

  Controller:update_window(sequence_number_acked);
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms( void )
{
  return 1000; /* timeout of one second */
}
