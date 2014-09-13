#include <cppunit/extensions/HelperMacros.h>
class basic_test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( basic_test );
  CPPUNIT_TEST( testEmpty );
  CPPUNIT_TEST_SUITE_END();
public:
  void testEmpty () { int i=5; CPPUNIT_ASSERT(i==5); }
};
CPPUNIT_TEST_SUITE_REGISTRATION( basic_test );
