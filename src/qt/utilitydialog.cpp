// Copyright (c) 2011-2021 The Bitcoin Core developers
// Copyright (c) 2014-2025 The Ratatoskr Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include <config/bitcoin-config.h>
#endif

#include <qt/utilitydialog.h>

#include <qt/forms/ui_helpmessagedialog.h>

#include <qt/guiutil.h>
#include <qt/guiutil_font.h>

#include <clientversion.h>
#include <init.h>
#include <util/system.h>
#include <util/strencodings.h>

#include <cstdio>

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QRegularExpression>
#include <QString>
#include <QTextCursor>
#include <QTextTable>
#include <QVBoxLayout>

/** "Help message" or "About" dialog box */
HelpMessageDialog::HelpMessageDialog(QWidget *parent, HelpMode helpMode) :
    QDialog(parent, GUIUtil::dialog_flags),
    ui(new Ui::HelpMessageDialog)
{
    ui->setupUi(this);

    GUIUtil::updateFonts();

    QString version = QString{PACKAGE_NAME} + " " + tr("version") + " " + QString::fromStdString(FormatFullVersion());

    if (helpMode == about)
    {
        setWindowTitle(tr("About %1").arg(PACKAGE_NAME));

        std::string licenseInfo = LicenseInfo();
        /// HTML-format the license message from the core
        QString licenseInfoHTML = QString::fromStdString(licenseInfo);

        // Make URLs clickable
        QRegularExpression uri(QStringLiteral("<(.*)>"), QRegularExpression::InvertedGreedinessOption);
        licenseInfoHTML.replace(uri, QString("<a style=\"%1\" href=\"\\1\">\\1</a>").arg(GUIUtil::getThemedStyleQString(GUIUtil::ThemedStyle::TS_COMMAND)));
        // Replace newlines with HTML breaks
        licenseInfoHTML.replace("\n", "<br>");

        ui->aboutMessage->setTextFormat(Qt::RichText);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        QString linkStyle = GUIUtil::getThemedStyleQString(GUIUtil::ThemedStyle::TS_COMMAND);

        // Tagline shown directly under the version line.
        QString tagline =
            "<i>" + tr("A yespower-mineable proof-of-work masternode chain "
                       "in the EnchantedForestDeFi ecosystem.") + "</i>";

        // Lineage acknowledgement — Bitcoin -> Dash -> Smartiecoin -> Ratatoskr.
        QString lineage = "<small>"
            + tr("Built on the work of the Bitcoin Core, Dash Core, and "
                 "Smartiecoin Core developers. The %1 codebase is forked "
                 "from Smartiecoin Core and remains under the MIT license.")
              .arg(QString{PACKAGE_NAME})
            + "</small>";

        QString ratrFooter = "<br><br>"
            "<b>" + tr("Ratatoskr resources") + "</b><br>"
            + tr("Website") + ": <a style=\"" + linkStyle + "\" href=\"https://ratatoskr.enchantedforestdefi.com\">ratatoskr.enchantedforestdefi.com</a><br>"
            + tr("Source code") + ": <a style=\"" + linkStyle + "\" href=\"https://github.com/EnchantedForestDeFi/ratatoskr\">github.com/EnchantedForestDeFi/ratatoskr</a><br>"
            + tr("Block explorer") + ": <a style=\"" + linkStyle + "\" href=\"https://explorer.ratatoskr.enchantedforestdefi.com\">explorer.ratatoskr.enchantedforestdefi.com</a><br>"
            + tr("Bridge to Alephium") + ": <a style=\"" + linkStyle + "\" href=\"https://ratatoskrbridge.enchantedforestdefi.com\">ratatoskrbridge.enchantedforestdefi.com</a><br>"
            + tr("Discord") + ": <a style=\"" + linkStyle + "\" href=\"https://discord.gg/SrffQVYqee\">discord.gg/SrffQVYqee</a>"
            + "<br><br>"
            + lineage;

        text = version + "\n" + QString::fromStdString(FormatParagraph(licenseInfo));
        ui->aboutMessage->setText(version + "<br>" + tagline + "<br><br>" + licenseInfoHTML + ratrFooter);
        ui->aboutMessage->setWordWrap(true);
        ui->helpMessage->setVisible(false);
    } else if (helpMode == cmdline) {
        setWindowTitle(tr("Command-line options"));
        QString header = "Usage: ratatoskr-qt [command-line options] [URI]\n\n"
                         "Optional URI is a Ratatoskr address in BIP21 URI format.\n";
        QTextCursor cursor(ui->helpMessage->document());
        cursor.insertText(version);
        cursor.insertBlock();
        cursor.insertText(header);
        cursor.insertBlock();

        std::string strUsage = gArgs.GetHelpMessage();
        QString coreOptions = QString::fromStdString(strUsage);
        text = version + "\n\n" + header + "\n" + coreOptions;

        QTextTableFormat tf;
        tf.setBorderStyle(QTextFrameFormat::BorderStyle_None);
        tf.setCellPadding(2);
        QVector<QTextLength> widths;
        widths << QTextLength(QTextLength::PercentageLength, 35);
        widths << QTextLength(QTextLength::PercentageLength, 65);
        tf.setColumnWidthConstraints(widths);

        QTextCharFormat bold;
        bold.setFontWeight(QFont::Bold);

        for (const QString &line : coreOptions.split("\n")) {
            if (line.startsWith("  -"))
            {
                cursor.currentTable()->appendRows(1);
                cursor.movePosition(QTextCursor::PreviousCell);
                cursor.movePosition(QTextCursor::NextRow);
                cursor.insertText(line.trimmed());
                cursor.movePosition(QTextCursor::NextCell);
            } else if (line.startsWith("   ")) {
                cursor.insertText(line.trimmed()+' ');
            } else if (line.size() > 0) {
                //Title of a group
                if (cursor.currentTable())
                    cursor.currentTable()->appendRows(1);
                cursor.movePosition(QTextCursor::Down);
                cursor.insertText(line.trimmed(), bold);
                cursor.insertTable(1, 2, tf);
            }
        }

        ui->helpMessage->moveCursor(QTextCursor::Start);
        ui->scrollArea->setVisible(false);
    } else if (helpMode == pshelp) {
        QString strCoinJoinName = QString::fromStdString(gCoinJoinName);
        setWindowTitle(tr("%1 information").arg(strCoinJoinName));

        ui->aboutMessage->setTextFormat(Qt::RichText);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->aboutMessage->setText(tr("\
<h3>%1 Basics</h3> \
%1 gives you true financial privacy by obscuring the origins of your funds. \
All the RATR in your wallet is comprised of different \"inputs\" which you can think of as separate, discrete coins.<br> \
%1 uses an innovative process to mix your inputs with the inputs of two or more other people, without having your coins ever leave your wallet. \
You retain control of your money at all times.<hr> \
<b>The %1 process works like this:</b>\
<ol type=\"1\"> \
<li>%1 begins by breaking your transaction inputs down into standard denominations. \
These denominations are 0.001 SMT, 0.01 SMT, 0.1 SMT, 1 SMT and 10 SMT -- sort of like the paper money you use every day.</li> \
<li>Your wallet then sends requests to specially configured software nodes on the network, called \"masternodes.\" \
These masternodes are informed then that you are interested in mixing a certain denomination. \
No identifiable information is sent to the masternodes, so they never know \"who\" you are.</li> \
<li>When two or more other people send similar messages, indicating that they wish to mix the same denomination, a mixing session begins. \
The masternode mixes up the inputs and instructs all three users' wallets to pay the now-transformed input back to themselves. \
Your wallet pays that denomination directly to itself, but in a different address (called a change address).</li> \
<li>In order to fully obscure your funds, your wallet must repeat this process a number of times with each denomination. \
Each time the process is completed, it's called a \"round.\" Each round of %1 makes it exponentially more difficult to determine where your funds originated.</li> \
<li>This mixing process happens in the background without any intervention on your part. When you wish to make a transaction, \
your funds will already be mixed. No additional waiting is required.</li> \
</ol> <hr>\
<b>IMPORTANT:</b> Your wallet only contains 1000 of these \"change addresses.\" Every time a mixing event happens, up to 9 of your addresses are used up. \
This means those 1000 addresses last for about 100 mixing events. When 900 of them are used, your wallet must create more addresses. \
It can only do this, however, if you have automatic backups enabled.<br> \
Consequently, users who have backups disabled will also have %1 disabled. <hr>\
For more information, see the <a style=\"%2\" href=\"%3\">%1 documentation</a>."
        )
        .arg(strCoinJoinName)
        .arg(GUIUtil::getThemedStyleQString(GUIUtil::ThemedStyle::TS_COMMAND))
        .arg("https://github.com/EnchantedForestDeFi/ratatoskr")
        );
        ui->aboutMessage->setWordWrap(true);
        ui->helpMessage->setVisible(false);
    } else if (helpMode == miningInfo) {
        setWindowTitle(tr("Mining %1").arg(PACKAGE_NAME));

        ui->aboutMessage->setTextFormat(Qt::RichText);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        QString linkStyle = GUIUtil::getThemedStyleQString(GUIUtil::ThemedStyle::TS_COMMAND);

        QString miningHtml =
            "<h3>" + tr("Mining %1").arg(PACKAGE_NAME) + "</h3>"
            + "<p>" + tr("%1 uses the <b>yespower</b> proof-of-work algorithm. yespower is "
                         "memory-hard and CPU-friendly, designed to remain economical to mine on "
                         "general-purpose hardware (laptops, desktops, and small servers).").arg(PACKAGE_NAME) + "</p>"

            + "<h4>" + tr("Pool mining (recommended)") + "</h4>"
            + "<p>" + tr("Most users should mine through a pool. Pools smooth out reward variance, "
                         "and a yespower miner pointed at a pool will start contributing within minutes.") + "</p>"
            + "<p>"
            + tr("Up-to-date list of %1 pools").arg(PACKAGE_NAME) + ": "
            + "<a style=\"" + linkStyle + "\" href=\"https://ratatoskr.enchantedforestdefi.com\">"
            + "ratatoskr.enchantedforestdefi.com</a>"
            + "</p>"

            + "<h4>" + tr("Solo mining") + "</h4>"
            + "<p>" + tr("Solo mining points a CPU miner at your own running %1 daemon. "
                         "Block reward variance is high — only viable if you control a substantial "
                         "fraction of network hashrate.").arg(PACKAGE_NAME) + "</p>"
            + "<p>" + tr("Example, using a yespower-aware build of cpuminer-opt:") + "</p>"
            + "<pre style=\"background:rgba(127,127,127,0.12);padding:6px;border-radius:4px\">"
              "cpuminer -a yespower \\\n"
              "  -o http://127.0.0.1:8282 \\\n"
              "  -u &lt;rpcuser&gt; -p &lt;rpcpassword&gt;\n"
              "  --coinbase-addr=&lt;your_RATR_receiving_address&gt;"
              "</pre>"
            + "<p>" + tr("Replace <code>&lt;rpcuser&gt;</code> and <code>&lt;rpcpassword&gt;</code> "
                         "with the values from your <code>ratatoskr.conf</code> file. Use the "
                         "<i>Receive</i> tab to copy a fresh address into <code>--coinbase-addr</code>.") + "</p>"
            + "<p>" + tr("Default RPC port is <b>8282</b> on mainnet (<b>18282</b> on testnet).") + "</p>"

            + "<h4>" + tr("Network mining stats") + "</h4>"
            + "<p>" + tr("Open <i>Tools &rarr; Debug console</i> and run <code>getmininginfo</code> "
                         "to see the current network hashrate, difficulty, and block height.") + "</p>"

            + "<h4>" + tr("Heads up") + "</h4>"
            + "<ul>"
            + "<li>" + tr("Mining will use most of your CPU and warm up your machine. Use a thread "
                         "limit (<code>-t N</code>) if you want to keep cycles free for other work.") + "</li>"
            + "<li>" + tr("Solo blocks credit your wallet directly. Pool payouts arrive at the "
                         "address you registered with the pool, not necessarily this wallet.") + "</li>"
            + "<li>" + tr("This dialog is informational only — %1 does not include a built-in CPU "
                         "miner. Use an external yespower miner (e.g. cpuminer-opt) instead.").arg(PACKAGE_NAME) + "</li>"
            + "</ul>";

        text = miningHtml;
        ui->aboutMessage->setText(miningHtml);
        ui->aboutMessage->setWordWrap(true);
        ui->helpMessage->setVisible(false);
    } else if (helpMode == welcome) {
        setWindowTitle(tr("Welcome to %1").arg(PACKAGE_NAME));

        ui->aboutMessage->setTextFormat(Qt::RichText);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        QString linkStyle = GUIUtil::getThemedStyleQString(GUIUtil::ThemedStyle::TS_COMMAND);
        auto link = [&linkStyle](const QString& url, const QString& label) {
            return QString("<a style=\"%1\" href=\"%2\">%3</a>")
                .arg(linkStyle, url, label);
        };

        QString welcomeHtml =
            "<h3>" + tr("Welcome to %1").arg(PACKAGE_NAME) + "</h3>"
            + "<p>" + tr("%1 is a yespower-mineable proof-of-work masternode "
                         "chain in the EnchantedForestDeFi ecosystem. In Norse "
                         "cosmology, Ratatoskr is the squirrel that runs up "
                         "and down Yggdrasil — the world tree — carrying "
                         "messages between realms. The project's bridge layer "
                         "is the modern incarnation of that role.").arg(PACKAGE_NAME) + "</p>"

            + "<h4>" + tr("First time here?") + "</h4>"
            + "<ul>"
            + "<li>" + tr("Use the <i>Receive</i> tab to generate an address "
                         "for incoming RATR.") + "</li>"
            + "<li>" + tr("Use the <i>Send</i> tab to send RATR to other "
                         "addresses.") + "</li>"
            + "<li>" + tr("If you want to mine, see <i>Help → Mining info</i> "
                         "for the yespower setup.") + "</li>"
            + "<li>" + tr("To move RATR onto Alephium as wRATR, use "
                         "<i>Help → Bridge to Alephium</i>.") + "</li>"
            + "<li>" + tr("Manage masternodes from the <i>Masternodes</i> "
                         "tab once you have the collateral and a configured "
                         "remote node.") + "</li>"
            + "</ul>"

            + "<h4>" + tr("The Alephium ↔ Alfheim connection") + "</h4>"
            + "<p>" + tr("The partner chain RATR bridges to is called "
                         "<b>Alephium</b> — phonetically cognate with "
                         "<b>Alfheim</b>, one of the upper realms of "
                         "Yggdrasil and home of the light elves. The "
                         "lore-mapping isn't a marketing stretch; the names "
                         "line up. Each future bridge endpoint becomes "
                         "another realm hanging from Yggdrasil's branches.") + "</p>"

            + "<h4>" + tr("Resources") + "</h4>"
            + "<ul>"
            + "<li>" + tr("Website") + ": "
              + link("https://ratatoskr.enchantedforestdefi.com",
                     "ratatoskr.enchantedforestdefi.com") + "</li>"
            + "<li>" + tr("Bridge to Alephium") + ": "
              + link("https://ratatoskrbridge.enchantedforestdefi.com",
                     "ratatoskrbridge.enchantedforestdefi.com") + "</li>"
            + "<li>" + tr("Block explorer") + ": "
              + link("https://explorer.ratatoskr.enchantedforestdefi.com",
                     "explorer.ratatoskr.enchantedforestdefi.com") + "</li>"
            + "<li>" + tr("Discord (community + support)") + ": "
              + link("https://discord.gg/SrffQVYqee",
                     "discord.gg/SrffQVYqee") + "</li>"
            + "<li>" + tr("Source code") + ": "
              + link("https://github.com/EnchantedForestDeFi/ratatoskr",
                     "github.com/EnchantedForestDeFi/ratatoskr") + "</li>"
            + "</ul>"

            + "<p><small>" + tr("This is experimental software. Not financial "
                                "advice. Read the whitepaper, run a testnet "
                                "node first if you're unsure, and never put "
                                "in more than you can afford to lose.") + "</small></p>";

        text = welcomeHtml;
        ui->aboutMessage->setText(welcomeHtml);
        ui->aboutMessage->setWordWrap(true);
        ui->helpMessage->setVisible(false);
    }

    GUIUtil::handleCloseWindowShortcut(this);
}

HelpMessageDialog::~HelpMessageDialog()
{
    delete ui;
}

void HelpMessageDialog::printToConsole()
{
    // On other operating systems, the expected action is to print the message to the console.
    tfm::format(std::cout, "%s", qPrintable(text));
}

void HelpMessageDialog::showOrPrint()
{
#if defined(WIN32)
    // On Windows, show a message box, as there is no stderr/stdout in windowed applications
    exec();
#else
    // On other operating systems, print help text to console
    printToConsole();
#endif
}

void HelpMessageDialog::on_okButton_accepted()
{
    close();
}


/** "Shutdown" window */
ShutdownWindow::ShutdownWindow(QWidget *parent, Qt::WindowFlags f):
    QWidget(parent, f)
{
    setObjectName("ShutdownWindow");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel(
        tr("%1 is shutting downâ€¦").arg(PACKAGE_NAME) + "<br /><br />" +
        tr("Do not shut down the computer until this window disappears.")));
    setLayout(layout);

    GUIUtil::updateFonts();

    GUIUtil::handleCloseWindowShortcut(this);
}

QWidget* ShutdownWindow::showShutdownWindow(QMainWindow* window)
{
    assert(window != nullptr);

    // Show a simple window indicating shutdown status
    QWidget *shutdownWindow = new ShutdownWindow();
    shutdownWindow->setWindowTitle(window->windowTitle());

    // Center shutdown window at where main window was
    const QPoint global = window->mapToGlobal(window->rect().center());
    shutdownWindow->move(global.x() - shutdownWindow->width() / 2, global.y() - shutdownWindow->height() / 2);
    shutdownWindow->show();
    return shutdownWindow;
}

void ShutdownWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
}
