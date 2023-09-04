#include <stdexcept>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) 
  , size_(0) , total_r(0) , total_w(0) , is_close(false) , is_error(false)
 {}

void Writer::push( string data )
{
  if(data.size() == 0 || available_capacity() == 0) return;

  int n = min(available_capacity() , data.size());
  
  if( n < (int)data.size())
    data = data.substr(0,n);

  q.push_back(data);
  size_ += n;
  total_w += n;

  (void)data;
}

void Writer::close()
{
  is_close = true;
}

void Writer::set_error()
{
  is_error = true;
}

bool Writer::is_closed() const
{
  return is_close ;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - size_ ;
}

uint64_t Writer::bytes_pushed() const
{
  return total_w ;
}

string_view Reader::peek() const
{
  if(q.empty()) return {};
  return q.front();
}

bool Reader::is_finished() const
{
  return is_close && size_ == 0;
}

bool Reader::has_error() const
{
  return is_error ;
}

void Reader::pop( uint64_t len )
{
  while(size_ && len){
    string view = q.front();
    q.pop_front();

    if(view.size() > len){
      view = view.substr( len , view.size()-len );
      q.push_front(view);
      size_ -= len;
      total_r += len;
      return;
    }
    
    len -= view.size();
    size_ -= view.size();
    total_r += view.size();  
    
  }
  
  (void)len;
}

uint64_t Reader::bytes_buffered() const
{
  return size_ ;
}

uint64_t Reader::bytes_popped() const
{
  return total_r ;
}
