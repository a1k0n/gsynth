; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "gsynth.h"
LastPage=0

ClassCount=10
Class1=CGsynthApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=9
Resource1=IDR_MENU1
Class2=CChildView
Resource2=IDR_MENU3
Resource3=IDD_DIALOG3
Resource4=IDR_MENU2
Class5=CAddModule
Resource5=IDD_SPLASHDLG
Class6=CRenameModule
Class7=CMachineWindow
Class8=CTrackerWindow
Resource6=IDD_DIALOG1
Class9=CCreatePattern
Resource7=IDD_ABOUTBOX
Class10=CSplash
Resource8=IDD_DIALOG2
Resource9=IDR_MAINFRAME

[CLS:CGsynthApp]
Type=0
HeaderFile=gsynth.h
ImplementationFile=gsynth.cpp
Filter=N

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=C
LastObject=CChildView
BaseClass=CWnd 
VirtualFilter=WC

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_FILE_OPEN




[CLS:CAboutDlg]
Type=0
HeaderFile=gsynth.cpp
ImplementationFile=gsynth.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_APP_EXIT
Command6=ID_EDIT_UNDO
Command7=ID_EDIT_CUT
Command8=ID_EDIT_COPY
Command9=ID_EDIT_PASTE
Command10=ID_VIEW_TOOLBAR
Command11=ID_VIEW_STATUS_BAR
Command12=ID_APP_ABOUT
CommandCount=12

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command4=ID_FILE_PRINT
Command5=ID_APP_ABOUT
Command6=ID_BUTTON32771
Command7=ID_BUTTON32772
CommandCount=7

[MNU:IDR_MENU1]
Type=1
Class=?
CommandCount=0

[DLG:IDD_DIALOG1]
Type=1
Class=CAddModule
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMBO1,combobox,1344340227
Control4=IDC_STATIC,static,1342308352

[MNU:IDR_MENU2]
Type=1
Class=?
CommandCount=0

[CLS:CAddModule]
Type=0
HeaderFile=AddModule.h
ImplementationFile=AddModule.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CAddModule

[DLG:IDD_DIALOG2]
Type=1
Class=CRenameModule
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552

[CLS:CRenameModule]
Type=0
HeaderFile=RenameModule.h
ImplementationFile=RenameModule.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK

[CLS:CMachineWindow]
Type=0
HeaderFile=MachineWindow.h
ImplementationFile=MachineWindow.cpp
BaseClass=CWnd
Filter=W
LastObject=CMachineWindow
VirtualFilter=WC

[CLS:CTrackerWindow]
Type=0
HeaderFile=TrackerWindow.h
ImplementationFile=TrackerWindow.cpp
BaseClass=CWnd
Filter=W
LastObject=CTrackerWindow
VirtualFilter=WC

[DLG:IDD_DIALOG3]
Type=1
Class=CCreatePattern
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552

[CLS:CCreatePattern]
Type=0
HeaderFile=CreatePattern.h
ImplementationFile=CreatePattern.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK

[DLG:IDD_SPLASHDLG]
Type=1
Class=CSplash
ControlCount=1
Control1=IDC_STATIC,static,1342177294

[CLS:CSplash]
Type=0
HeaderFile=Splash1.h
ImplementationFile=Splash1.cpp
BaseClass=CDialog
Filter=D
LastObject=CSplash
VirtualFilter=dWC

[MNU:IDR_MENU3]
Type=1
Class=?
CommandCount=0

