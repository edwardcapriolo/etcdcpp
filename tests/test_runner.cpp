#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace CppUnit;

int main (int argc, char* argv[]) {
  TextUi::TestRunner runner;
  TestFactoryRegistry& registry = TestFactoryRegistry::getRegistry();

  // run all tests if none specified on command line 
  Test* test_to_run = registry.makeTest();
  if (argc>1)
        test_to_run = test_to_run->findTest(argv[1]);

  runner.addTest( test_to_run );
  bool failed = runner.run("", false);
  return !failed;
}
