#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Qt
#include <QApplication>
#include <QFileInfo>
#include <QDir>

// MythTV
#include "mythcontext.h"
#include "mythversion.h"
#include "mythtranslation.h"
#include "mythdbcon.h"
#include "exitcodes.h"
#include "compat.h"
#include "lcddevice.h"
#include "commandlineparser.h"
#include "tv.h"
#include "mythlogging.h"

// libmythui
#include "mythmainwindow.h"
#include "mythuihelper.h"

// mythwelcome
#include "welcomedialog.h"
#include "welcomesettings.h"


QString logfile;


static void initKeys(void)
{
    REG_KEY("Welcome", "STARTXTERM", QT_TRANSLATE_NOOP("MythControls",
        "Open an Xterm window"),       "F12");
    REG_KEY("Welcome", "SHOWSETTINGS", QT_TRANSLATE_NOOP("MythControls",
        "Show Mythshutdown settings"), "F11");
    REG_KEY("Welcome", "STARTSETUP", QT_TRANSLATE_NOOP("MythControls",
        "Start Mythtv-Setup"),            "");
}

int main(int argc, char **argv)
{
    bool bShowSettings = false;

    MythWelcomeCommandLineParser cmdline;
    if (!cmdline.Parse(argc, argv))
    {
        cmdline.PrintHelp();
        return GENERIC_EXIT_INVALID_CMDLINE;
    }

    if (cmdline.toBool("showhelp"))
    {
        cmdline.PrintHelp();
        return GENERIC_EXIT_OK;
    }

    if (cmdline.toBool("showversion"))
    {
        cmdline.PrintVersion();
        return GENERIC_EXIT_OK;
    }
    
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(MYTH_APPNAME_MYTHWELCOME);

    int retval;
    if ((retval = cmdline.ConfigureLogging()) != GENERIC_EXIT_OK)
        return retval;

    if (cmdline.toBool("setup"))
        bShowSettings = true;

    gContext = new MythContext(MYTH_BINARY_VERSION);
    if (!gContext->Init())
    {
        LOG(VB_GENERAL, LOG_ERR,
            "mythwelcome: Could not initialize MythContext. Exiting.");
        return GENERIC_EXIT_NO_MYTHCONTEXT;
    }

    if (!MSqlQuery::testDBConnection())
    {
        LOG(VB_GENERAL, LOG_ERR,
            "mythwelcome: Could not open the database. Exiting.");
        return -1;
    }

    LCD::SetupLCD();

    if (LCD *lcd = LCD::Get())
        lcd->switchToTime();

    MythTranslation::load("mythfrontend");

    GetMythUI()->LoadQtConfig();

#ifdef Q_WS_MACX
    // Mac OS 10.4 and Qt 4.4 have window-focus problems
    gCoreContext->SetSetting("RunFrontendInWindow", "1");
#endif

    MythMainWindow *mainWindow = GetMythMainWindow();
    mainWindow->Init();

    initKeys();

    if (bShowSettings)
    {
        MythShutdownSettings settings;
        settings.exec();
    }
    else
    {
        MythScreenStack *mainStack = GetMythMainWindow()->GetMainStack();

        WelcomeDialog *welcome = new WelcomeDialog(mainStack, "mythwelcome");

        if (welcome->Create())
            mainStack->AddScreen(welcome, false);
        else
            return -1;

        do
        {
            qApp->processEvents();
            usleep(5000);
        } while (mainStack->TotalScreens() > 0);
    }

    DestroyMythMainWindow();

    delete gContext;

    return 0;
}
