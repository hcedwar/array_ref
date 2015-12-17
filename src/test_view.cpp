
#include <view.hpp>
#include <layouts.hpp>
#include <iostream>
#include <typeinfo>
#include <cassert>

template <std::size_t ... sizes>
struct my_favorite_layout
{
    using is_layout = std::true_type;
    using dimension = std::experimental::view_property::dimension<sizes...>;
    dimension dim;

  using is_regular = std::true_type ;

  template< typename t0 >
  constexpr size_t operator()( const t0 & i0 ) const
    { return i0 ; }

  template< typename t0 , typename t1 >
  constexpr size_t operator()( const t0 & i0 , const t1 & i1 ) const
    { return i0 + dim.n0 * i1 ; }

  template< typename t0 , typename t1 , typename t2 >
  constexpr size_t operator()( const t0 & i0 , const t1 & i1 , const t2 & i2 ) const
    { return i0 + dim.n0 * ( i1 + dim.n1 * i2 ); }

  constexpr size_t stride_0() const { return 1 ; }
  constexpr size_t stride_1() const { return dim.n0 ; }
  constexpr size_t stride_2() const { return dim.n0 * dim.n1 ; }

  constexpr size_t span() const
    { return dim.n0 * dim.n1 * dim.n2 * dim.n3 * dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }

 my_favorite_layout() = default ;
  my_favorite_layout( const my_favorite_layout & ) = default ;
  my_favorite_layout( my_favorite_layout && ) = default ;
  my_favorite_layout & operator = ( const my_favorite_layout & ) = default ;
  my_favorite_layout & operator = ( my_favorite_layout && ) = default ;

  constexpr my_favorite_layout( size_t a0     , size_t a1 = 0 , size_t a2 = 0 , size_t a3 = 0 , size_t a4 = 0
                       , size_t a5 = 0 , size_t a6 = 0 , size_t a7 = 0 , size_t a8 = 0 , size_t a9 = 0 )
    : dim(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) {}
};

int main()
{
  using namespace std::experimental ;
  using namespace std::experimental::view_property ;

  // Verify only implicit dimensions have variables.
  std::cout << " sizeof(dimension<>) = " << sizeof(dimension<>) << std::endl ;
  std::cout << " sizeof(dimension<1>) = " << sizeof(dimension<1>) << std::endl ;
  std::cout << " sizeof(dimension<0>) = " << sizeof(dimension<0>) << std::endl ;
  std::cout << " sizeof(dimension<0,0>) = " << sizeof(dimension<0,0>) << std::endl ;
  std::cout << " sizeof(dimension<0,0,0>) = " << sizeof(dimension<0,0,0>) << std::endl ;
  std::cout << " sizeof(dimension<1,0,0>) = " << sizeof(dimension<1,0,0>) << std::endl ;

  // Verify dimension unpacking:
  std::cout << dimension<>() << std::endl ;
  std::cout << dimension<1,2,3,4,5,6,7,8,9,10>() << std::endl ;
  std::cout << dimension<0,2,3,4,5,6,7,8,9,10>(11) << std::endl ;
  std::cout << dimension<1,0,3,4,5,6,7,8,9,10>(12) << std::endl ;
  std::cout << dimension<1,2,0,4,5,6,7,8,9,10>(13) << std::endl ;
  std::cout << dimension<1,2,3,0,5,6,7,8,9,10>(14) << std::endl ;
  std::cout << dimension<1,2,3,4,0,6,7,8,9,10>(15) << std::endl ;
  std::cout << dimension<0,2,3,4,0,6,7,8,9,10>(11,15) << std::endl ;
  std::cout << dimension<0,2,3,4,0>(11,15) << std::endl ;

  // Can construct 'int[][][]' type:
  typedef implicit_array_type<int,0>::type t0 ;
  typedef implicit_array_type<int,1>::type t1 ;
  typedef implicit_array_type<int,2>::type t2 ;

  std::cout << typeid(t0).name() << std::endl ;
  std::cout << typeid(t1).name() << std::endl ;
  std::cout << typeid(t2).name() << std::endl ;

  // Verify extent can be evaluated at compilation
  // for 'constexpr' constructed view.
  static_assert( view<int, layout_offset_void<dimension<0,4> > >().extent(0) == 0 , "" );
  static_assert( view<int, layout_offset_left<dimension<3,10> > >().extent(1) == 10 , "" );
  static_assert( view<int, layout_offset_right<dimension<3,0,1> > >().extent(2) == 1 , "" );

  using my_view = view<int, my_favorite_layout<5,0,10,4,0> >;
  static_assert( my_view().extent(0) == 5 , "" );
  static_assert( my_view().extent(1) == 0 , "" );
  static_assert( my_view().extent(2) == 10 , "" );
  static_assert( my_view().extent(3) == 4 , "" );
  static_assert( my_view().extent(4) == 0 , "" );

  int v_data[100];
                 my_view v(v_data, 666, 999);
  assert( v.extent(1) == 666 );
  assert( v.extent(4) == 999 );

  // Verify view construction
  {
    int x_data[100];
    view< int, layout_offset_right<dimension<0,10> > > x(x_data,9);
    static_assert( x.rank() == 2 , "" );
    assert( x.extent(0) == 9 );
    static_assert( x.extent(1) == 10 , "" );
    // x(5) = 41 ; // no match due to insufficient rank
    x(5,5) = 42 ;   // matches proper rank
    x(5,5,0) = 43 ; // matches improper rank
  }

  return 0 ;
}

