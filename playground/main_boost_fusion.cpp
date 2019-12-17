#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/include/zip.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/mpl/range_c.hpp>

#include <boost/type_index.hpp>

#include <cstdint>
#include <cstdlib>

#include <iostream>
#include <string>

namespace fusion = boost::fusion;
namespace mpl    = boost::mpl;

struct Foo
{
    int x;
    int y;
    double z;
};

BOOST_FUSION_ADAPT_STRUCT(Foo, x, y, z);

struct print_xml
{

    void operator()(Foo const& foo) const
    {
        std::cout                            //
            << "<Foo>"                       //
            << "<int>" << foo.x << "/<int>"  //
            << "<int>" << foo.y << "/<int>"  //
            << "<int>" << foo.z << "/<int>"  //
            << "</Foo>"                      //
            ;                                //
    }

    template<typename T>
    void operator()(T const& x) const
    {
        std::cout                               //
            << '<' << typeid(x).name() << '>'   //
            << x                                //
            << "</" << typeid(x).name() << '>'  //
            ;                                   //
    }
};

int main(int, char**)
{
    auto stuff = fusion::vector<Foo, char, int64_t>(Foo{143, 0, 2.3}, 'x', 123);
    fusion::for_each(stuff, print_xml());

    return EXIT_SUCCESS;
}
