QT       += core gui \
            xml \
            charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    package/Func.cpp \
    package/FutuQuota.cpp \
    package/FutuTrade.cpp \
    user/account.cpp \
    widget/bargaintable.cpp \
    widget/buy/buymodel.cpp \
    widget/buy/buytable.cpp \
    widget/chart/stockchart.cpp \
    widget/clickedlabel.cpp \
    widget/clickedwidget.cpp \
    widget/crosswidget.cpp \
    widget/entrusttable.cpp \
    widget/freezetablewidget.cpp \
    widget/logindialog.cpp \
    widget/mytablemodel.cpp \
    widget/positiontable.cpp \
    widget/quotadialog.cpp \
    widget/quotaheadertable.cpp \
    widget/quotatable.cpp \
    widget/sell/sellmodel.cpp \
    widget/sell/selltable.cpp \
    widget/stockwindow.cpp

HEADERS += \
    Func.h \
    FutuTrade.h \
    fututrade.h \
    package/CTradeImplInterface.h \
    package/Func.h \
    package/FutuQuota.h \
    package/FutuTrade.h \
    package/HsFutuSystemInfo.h \
    package/t2sdk_interface.h \
    public/Basic.h \
    public/QuotaStruct.h \
    user/account.h \
    user/accountinterface.h \
    widget/bargaintable.h \
    widget/buy/buymodel.h \
    widget/buy/buytable.h \
    widget/chart/stockchart.h \
    widget/clickedlabel.h \
    widget/clickedwidget.h \
    widget/crosswidget.h \
    widget/entrusttable.h \
    widget/freezetablewidget.h \
    widget/logindialog.h \
    widget/mytablemodel.h \
    widget/positiontable.h \
    widget/quotadialog.h \
    widget/quotaheadertable.h \
    widget/quotatable.h \
    widget/sell/sellmodel.h \
    widget/sell/selltable.h \
    widget/stockwindow.h

FORMS += \
    account.ui \
    widget/logindialog.ui \
    widget/quotadialog.ui \
    widget/stockwindow.ui

# 外部库
LIBS = -L D:/lucky/lib/ -lt2sdk -lHsFutuSystemInfo

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE += \
    logo/logo.rc
