/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_port_ext.h                   |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                           Taktak Sami                       |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/

#ifndef __SC_PORT_EXT_H__
#define __SC_PORT_EXT_H__

// Define registers writing method
#include <iostream>
#include <cstdlib>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "internal_ext.h"
#include "port_dependency_ext.h"
#include "sc_event.h"
#include "sc_fwd.h"
#include "sc_interface.h"
#include "sc_nbdefs.h"
#include "sc_object.h"


//__GNUC__
//__GNUC_MINOR__
//__GNUC_PATCHLEVEL__
//
#if ((__GNUC__ < 3) || (__GNUC_MINOR__ < 4))
    #define INLINE __attribute__((always_inline))
#else
    /* gcc3.4 doesn't support */ 
    #define INLINE
#endif

#include <list>


namespace sc_core {

using namespace sc_dt;

const char * get_name(const tab_t * pointer);

#define READ_SIGNAL(value_type_,pointer_) \
  ((value_type_&) (*((value_type_*) (pointer_))))


///////////////////// DEPRECATED
// C ANSI-only since it is needed to link with extern "C"


extern void bind(sc_port_base &, sc_port_base&);
extern void bind(sc_port_base &, sc_signal_base&);
extern void bind(sc_signal_base & x);
extern void bind(sc_port_base & x);

// KIND STRING
extern const char * const sc_inout_string;
extern const char * const sc_in_string;
extern const char * const sc_out_string;  

extern "C" void update(void);

class sc_port_base : public sc_object, public sc_interface {

    protected:
    typedef sc_port_base base_type;

    public:
    ///////////
    // Internal
    const sc_module & get_module() const;
    void init();
    void check_multiwriting2port() const;
    ///////////     

    friend std::ostream & operator << (std::ostream &, const sc_port_base &);


    // LRM
    static const char * const kind_string;
    //

    sc_port_base();
    sc_port_base(const char * name_);
    explicit sc_port_base(const sc_port_base & parent_);
    /*virtual */~sc_port_base() {};
    // bind to a handle
    void operator () (method_process_t & func) const;
};


template < typename T > class sc_port_b : public sc_port_base
{
};


// ----------------------------------------------------------------------------
//  CLASS : sc_in< T >
//
// ----------------------------------------------------------------------------

template < typename T >
class sc_in : public sc_port_base {

    private:
    typedef T data_type;
    typedef sc_port_base base_type;
    typedef sc_in<data_type> this_type;
    typedef sc_signal<data_type> signal_type;


    ///////////
    // Internal
    void init();
    ///////////

    public:
    // constructors
    sc_in() : base_type() { init(); }

    explicit sc_in(const char * name_) : base_type(name_) { init(); }

    explicit sc_in(const base_type & parent_) : base_type( parent_ ) { init(); }

    /*
    // LRM error !
    //static const char *const kind_string; this is a template !
    */
    //  virtual const char *kind () const 
    //  { return "sc_in"; };

    sc_event neg() const { return sc_event(*this, sc_event::NEG); };
    sc_event pos() const { return sc_event(*this, sc_event::POS); };

    // read the current value
    inline const T & read() const INLINE;  
    inline operator const T & () const INLINE;

    // operateur ==
    inline bool operator == (const T & v) INLINE;

    // bind to in interface
    void operator () (sc_signal<data_type> & s) {
        sc_core::bind(*this, s);
    }

    // binding for hierarchical description
    void operator () (this_type & parent_) {
        sc_core::bind(*this, parent_);
    }

    void operator () (sc_out<data_type> & o) {
        sc_core::bind(*this, o);
    }

    /*virtual */~sc_in() {};

};


template < typename T >
void sc_in< T >::init() { 
    set_kind(sc_in_string);
    sc_interface::init(sizeof(data_type)); 
}


// read
template < typename T > inline 
const T & sc_in< T >::read() const { 
#ifdef DUMP_READ
    std::cerr << "read " << READ_SIGNAL(const T &, get_pointer())
     << " on signal " << name() << "\n";
#endif
    return READ_SIGNAL(const T, get_pointer());
}
  

template < typename T > inline 
sc_in< T >::operator const T & () const {
    return sc_in< T >::read();
}


template < typename T > inline 
bool sc_in< T >::operator == (const T & v) {
    return sc_in< T >::read() == v;
}



// ----------------------------------------------------------------------------
//  CLASS : sc_inout< T >
//
// ----------------------------------------------------------------------------

template < typename  T >
class sc_inout : public sc_port_base {

  ///////////
  // Internal
    protected:
    void init();
    T val;

    private:
    typedef T data_type;

    typedef sc_inout<data_type> this_type;
    typedef sc_signal<data_type> signal_type;

    
    public:
    // contructeurs
    sc_inout() : base_type() { init (); };

    explicit sc_inout(const char * name_) : base_type(name_) { init(); };

    /*
    // LRM error !
    //static const char *const kind_string; this is a template !
    */

    // read the current value
    inline const T & read() const INLINE;
    // write the new value
    inline void write(const T &) INLINE;
    template < int W > inline void write(const sc_uint< W > & v) {
        sc_inout< T >::write(v.read());
    }

    inline operator const T & () const INLINE;

    inline sc_inout< T > & operator = (const T & a) INLINE;
    inline sc_inout< T > & operator = (const sc_signal< T > & a) INLINE;

    // operateur ==
    inline bool operator == (const bool & v) INLINE;

    // bind to in interface
    void operator () (sc_signal<data_type> & s) {
        bind(*this);
        bind(*this, s);
    }

    void operator () (this_type & parent_) {
        bind(*this, parent_);
    }

    /*virtual */~sc_inout() {};
};


template < typename T >
void sc_inout< T >::init() {
    set_pointer((tab_t *) (void *) &val);
    sc_object::set_kind(sc_inout_string);
    sc_interface::init(sizeof(data_type)); 
    /*ref*/ val = (0);
    //sc_inout::write (0);
    /* Fix : 
     * FSM checker generates an error at runtime 
     */

}


// read
template < typename T >
inline const T & sc_inout< T >::read() const {
#ifdef DUMP_READ
    std::cerr << "read " << READ_SIGNAL(const T, get_pointer()) // val
        << " on signal " << name () << "\n";
#endif
    //  return val; 
    return READ_SIGNAL(const T, get_pointer());
}


// write the new value
template < typename T >
inline void sc_inout< T >::write(const T & value_) {
#ifdef DUMP_WRITE
    std::cerr << "write " << value_ << " on in/out port (writing into a signal) '" << name() << "'\n";
#endif
    //  T& ref = *(T*)(get_pointer());
#ifndef USE_PORT_DEPENDENCY
    unstable |= (value_) != val; //ref;
#endif
    /*ref*/ val = (value_);
}


template < typename T >
inline sc_inout< T >::operator const T & () const {
    return sc_inout< T >::read();
}


template < typename T >
inline sc_inout< T > & sc_inout< T >::operator = (const T & a) {
    sc_inout< T >::write(a);
    return *this;
}


template < typename T >
inline sc_inout< T > & sc_inout< T >::operator = (const sc_signal< T > & a) {
    sc_inout< T >::write(a.read());
    return *this;
}


/*
   template <typename T>
   inline 
   sc_inout<T>& sc_inout<T>::operator = ( const sc_port_base& a )
   { write( a.read() ); return *this; }
   */


template < typename T > inline 
bool sc_inout< T >::operator == (const bool & v) {
    return sc_inout< T >::read() == v;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_out<T>
//
// ----------------------------------------------------------------------------
// Official SystemC implementation notes :
// "sc_out can also read from its port, hence no difference with sc_inout.
// For debugging reasons, a class is provided instead of a define."

template < typename T >
class sc_out : public sc_inout< T > {

    ///////////
    // Internal
    void init();
    ///////////
  
    public:
    typedef T data_type;
    typedef sc_inout< T > base_type;
    typedef sc_out<data_type> this_type;
    typedef sc_signal<data_type> signal_type;


    // constructors & destructor 
    sc_out() : base_type() {
        init();
    }

    explicit sc_out(const char * name_) : base_type(name_) {
        init();
    }

    sc_out(this_type & parent_);
    sc_out(const char * name_, this_type & parent_);

    /*
    // LRM error !
    //static const char *const kind_string; this is a template !
    */
    //virtual const char *kind () const 
    //{ return "sc_out"; };


    inline this_type & operator = (const data_type & a) INLINE;
    inline bool operator == (const bool & v) INLINE;

    // bind to in interface
    void operator () (sc_signal<data_type> & s) {
        bind(*this, s);
    }

    void operator () (this_type & parent_) {
        bind(*this, parent_);
    }

    //////////////////////
    // Systemcass specific
    void operator () (sc_port_base & o) {
        set_port_dependency(&o, (sc_port_base &) (*this));
    }
    //////////////////////


    /*virtual */~sc_out() {};

    private:
    // disabled 
    sc_out(const this_type &);

};



template< typename T >
void sc_out< T >::init() {
    sc_inout< T >::init();
    //  tab_t *t = &(sc_inout<T>::val);
    //  sc_interface::set_pointer (t);
    sc_object::set_kind(sc_out_string);
    //  sc_interface::init (sizeof (data_type)); 
    //  /*ref*/ sc_inout<T>::val = 0;
    //sc_inout<T>::write (0); 
    /* Fix : 
     * FSM checker generates an error at runtime 
     */
}


template< typename T >
inline sc_out< T > & sc_out< T >::operator = (const data_type & a) {
    sc_out< T >::write(a);
    return *this;
}


template< typename T >
inline bool sc_out< T >::operator == (const bool & v) {
    return sc_out< T >::read() == v;
}


// Dumps
template< typename T >
inline std::ostream & operator << ( std::ostream & os, const sc_in< T > & r) {
    return os << r.read();
}


// Add '&'
template< class T >
inline std::ostream & operator << ( std::ostream & os, const sc_inout< T > & r) {
    return os << r.read();
}


template< class T >
inline std::ostream & operator << ( std::ostream & os, const sc_signal< T > & r) {
    return os << r.read();
}


// Declarations
typedef sc_in<bool> sc_in_clk;

#undef INLINE

#undef READ_SIGNAL

} // end of sc_core namespace

using sc_core::sc_in_clk;

#endif /* __SC_PORT_EXT_H__ */

/*
# Local Variables:
# tab-width: 4;
# c-basic-offset: 4;
# c-file-offsets:((innamespace . 0)(inline-open . 0));
# indent-tabs-mode: nil;
# End:
#
# vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4
*/

