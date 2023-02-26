#include <QtTest>
#include "../../cirrus-list/mapmanager.h"
// add necessary includes here
//http://blog.davidecoppola.com/2017/11/cpp-unit-testing-with-qt-test-introduction/
//https://stuff.mit.edu/afs/athena/software/texmaker_v5.0.2/qt57/doc/qttestlib/qtest-overview.html

class cirrus_tests : public QObject
{
    Q_OBJECT

public:
    cirrus_tests();
    ~cirrus_tests();

private slots:
    void test_case0();
    void test_case2();

    /*
    // called before the first test function
    void initTestCase();
    // called before every test function
    void init();
    // called after every test function
    void cleanup();
    // called after the last test function
    void cleanupTestCase();
    */

};

cirrus_tests::cirrus_tests()
{

}

cirrus_tests::~cirrus_tests()
{

}

void cirrus_tests::test_case0()
{
    QVERIFY(1==1);
    //QVERIFY2(1==2, "one doesn't match two");
    //QCOMPARE(1, 2);
    //QVERIFY_EXCEPTION_THROWN
    //QTRY_VERIFY_WITH_TIMEOUT

}

void cirrus_tests::test_case2()
{
    Mapmanager mapManager;
    QString map = mapManager.makeNewMap("testme");
    QVERIFY(!map.isEmpty());
QVERIFY(1==1);
}

QTEST_APPLESS_MAIN(cirrus_tests)

//#include "tst_cirrus_tests.moc"
