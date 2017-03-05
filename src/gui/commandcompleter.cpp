/*
    Copyright (c) 2017, Lukas Holecek <hluk@email.cz>

    This file is part of CopyQ.

    CopyQ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CopyQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CopyQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "commandcompleter.h"
#include "commandsyntaxhighlighter.h"

#include <QAbstractItemView>
#include <QCompleter>
#include <QCoreApplication>
#include <QHash>
#include <QHeaderView>
#include <QKeyEvent>
#include <QTableView>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QStringListModel>
#include <QShortcut>

namespace {

QStringList scriptableCompletions()
{
    return scriptableObjects()
         + scriptableProperties()
         + scriptableFunctions()
         + scriptableKeywords();
}

class CommandCompleterModel : public QStringListModel {
public:
    explicit CommandCompleterModel(QObject *parent)
        : QStringListModel(scriptableCompletions(), parent)
    {
        for (const auto &name : scriptableObjects())
            m_doc[name].tag = "t";

        for (const auto &name : scriptableProperties())
            m_doc[name].tag = "o";

        for (const auto &name : scriptableFunctions())
            m_doc[name].tag = "fn";

        for (const auto &name : scriptableKeywords())
            m_doc[name].tag = "k";

        addDocumentation();
    }

    int columnCount(const QModelIndex &) const override
    {
        return 3;
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        const int row = index.row();

        if (index.column() == 1) {
            if (role == Qt::DisplayRole || role == Qt::EditRole)
                return QString(documentationForRow(row).tag);

            if (role == Qt::ForegroundRole)
                return QColor(Qt::gray);

            return QVariant();
        }

        if (index.column() == 2) {
            if (role == Qt::DisplayRole || role == Qt::EditRole)
                return documentationForRow(row).doc;

            return QVariant();
        }

        if (role == Qt::ToolTipRole)
            return typeForRow(row);

        return QStringListModel::data(index, role);
    }

private:
    struct ScriptableDocumentation {
        QString tag;
        QString doc;
    };

    // Generated by "utils/script_docs_to_cpp.py" from "src/scriptable/README.md".
    void addDocumentation()
    {
        addDocumentation("version", "String version()", "Returns version string.");
        addDocumentation("help", "String help()", "Returns help string.");
        addDocumentation("help", "String help(searchString, ...)", "Returns help for matched commands.");
        addDocumentation("show", "show()", "Shows main window.");
        addDocumentation("show", "show(tabName)", "Shows tab.");
        addDocumentation("showAt", "showAt()", "Shows main window under mouse cursor.");
        addDocumentation("showAt", "showAt(x, y, [width, height])", "Shows main window with given geometry.");
        addDocumentation("showAt", "showAt(x, y, width, height, tabName)", "Shows tab with given geometry.");
        addDocumentation("hide", "hide()", "Hides main window.");
        addDocumentation("toggle", "bool toggle()", "Shows or hides main window.");
        addDocumentation("menu", "menu()", "Opens context menu.");
        addDocumentation("menu", "menu(tabName, [maxItemCount])", "Shows context menu for given tab.");
        addDocumentation("exit", "exit()", "Exits server.");
        addDocumentation("disable", "disable(), enable()", "Disables or enables clipboard content storing.");
        addDocumentation("monitoring", "bool monitoring()", "Returns true only if clipboard storing is enabled.");
        addDocumentation("visible", "bool visible()", "Available since v2.4.7.");
        addDocumentation("focused", "bool focused()", "Available since v2.4.9.");
        addDocumentation("filter", "filter(filterText)", "Available since v2.4.9.");
        addDocumentation("ignore", "ignore()", "Ignores current clipboard content (used for automatic commands).");
        addDocumentation("clipboard", "ByteArray clipboard([mimeType])", "Returns clipboard data for MIME type (default is text).");
        addDocumentation("selection", "ByteArray selection([mimeType])", "Same as `clipboard()` for Linux/X11 mouse selection.");
        addDocumentation("copy", "bool copy(text)", "Sets clipboard plain text.");
        addDocumentation("copy", "bool copy(mimeType, data, [mimeType, data]...)", "Sets clipboard data.");
        addDocumentation("copy", "bool copy()", "Sends `Ctrl+C` to current window.");
        addDocumentation("copySelection", "ByteArray copySelection(...)", "Same as `copy(...)` for Linux/X11 mouse selection.");
        addDocumentation("paste", "paste()", "Pastes current clipboard.");
        addDocumentation("tab", "Array tab()", "Returns array of with tab names.");
        addDocumentation("tab", "tab(tabName)", "Sets current tab for the script.");
        addDocumentation("removeTab", "removeTab(tabName)", "Removes tab.");
        addDocumentation("renameTab", "renameTab(tabName, newTabName)", "Renames tab.");
        addDocumentation("tabIcon", "String tabIcon(tabName)", "Returns path to icon for tab.");
        addDocumentation("tabIcon", "tabIcon(tabName, iconPath)", "Sets icon for tab.");
        addDocumentation("count", "count(), length(), size()", "Returns amount of items in current tab.");
        addDocumentation("select", "select(row)", "Copies item in the row to clipboard.");
        addDocumentation("next", "next()", "Copies next item from current tab to clipboard.");
        addDocumentation("previous", "previous()", "Copies previous item from current tab to clipboard.");
        addDocumentation("add", "add(text, ...)", "Adds new text items to current tab.");
        addDocumentation("insert", "insert(row, text)", "Inserts new text items to current tab.");
        addDocumentation("remove", "remove(row, ...)", "Removes items in current tab.");
        addDocumentation("edit", "edit([row|text] ...)", "Edits items in current tab.");
        addDocumentation("read", "ByteArray read([mimeType]);", "Same as `clipboard()`.");
        addDocumentation("read", "ByteArray read(mimeType, row, ...);", "Returns concatenated data from items.");
        addDocumentation("write", "write(row, mimeType, data, [mimeType, data]...)", "Inserts new item to current tab.");
        addDocumentation("change", "change(row, mimeType, data, [mimeType, data]...)", "Changes data in item in current tab.");
        addDocumentation("separator", "String separator()", "Returns item separator (used when concatenating item data).");
        addDocumentation("separator", "separator(separator)", "Sets item separator for concatenating item data.");
        addDocumentation("action", "action()", "Opens action dialog.");
        addDocumentation("action", "action(row, ..., command, outputItemSeparator)", "Runs command for items in current tab.");
        addDocumentation("popup", "popup(title, message, [time=8000])", "Shows popup message for given time in milliseconds.");
        addDocumentation("exportTab", "exportTab(fileName)", "Exports current tab into file.");
        addDocumentation("importTab", "importTab(fileName)", "Imports items from file to a new tab.");
        addDocumentation("config", "String config()", "Returns help with list of available options.");
        addDocumentation("config", "String config(optionName)", "Returns value of given option.");
        addDocumentation("config", "String config(optionName, value)", "Sets option and returns new value.");
        addDocumentation("config", "String config(optionName, value, ...)", "Sets multiple options and return list with values in format `optionName=newValue`.");
        addDocumentation("info", "String info([pathName])", "Returns paths and flags used by the application.");
        addDocumentation("eval", "Value eval(script)", "Evaluates script and returns result.");
        addDocumentation("currentPath", "String currentPath([path])", "Get or set current path.");
        addDocumentation("str", "String str(value)", "Converts a value to string.");
        addDocumentation("input", "ByteArray input()", "Returns standard input passed to the script.");
        addDocumentation("data", "ByteArray data(mimeType)", "Returns data for automatic commands or selected items.");
        addDocumentation("setData", "ByteArray setData(mimeType, data)", "Modifies data for `data()` and new clipboard item.");
        addDocumentation("removeData", "ByteArray removeData(mimeType)", "Removes data for `data()` and new clipboard item.");
        addDocumentation("dataFormats", "Array dataFormats()", "Returns formats available for `data()`.");
        addDocumentation("print", "print(value)", "Prints value to standard output.");
        addDocumentation("abort", "abort()", "Aborts script evaluation.");
        addDocumentation("fail", "fail()", "Aborts script evaluation with nonzero exit code.");
        addDocumentation("setCurrentTab", "setCurrentTab(tabName)", "Focus tab without showing main window.");
        addDocumentation("selectItems", "selectItems(row, ...)", "Selects items in current tab.");
        addDocumentation("selectedTab", "String selectedTab()", "Returns tab that was selected when script was executed.");
        addDocumentation("selectedItems", "[row, ...] selectedItems()", "Returns selected rows in current tab.");
        addDocumentation("currentItem", "int currentItem(), int index()", "Returns current row in current tab.");
        addDocumentation("escapeHtml", "String escapeHtml(text)", "Returns text with special HTML characters escaped.");
        addDocumentation("unpack", "Item unpack(data)", "Returns deserialized object from serialized items.");
        addDocumentation("pack", "ByteArray pack(item)", "Returns serialized item.");
        addDocumentation("getItem", "Item getItem(row)", "Returns an item in current tab.");
        addDocumentation("setItem", "setItem(row, item)", "Inserts item to current tab.");
        addDocumentation("toBase64", "String toBase64(data)", "Returns base64-encoded data.");
        addDocumentation("fromBase64", "ByteArray fromBase64(base64String)", "Returns base64-decoded data.");
        addDocumentation("open", "QScriptValue open(url, ...)", "Tries to open URLs in appropriate applications.");
        addDocumentation("execute", "FinishedCommand execute(argument, ..., null, stdinData, ...)", "Executes a command.");
        addDocumentation("currentWindowTitle", "String currentWindowTitle()", "Returns window title of currently focused window.");
        addDocumentation("dialog", "Value dialog(...)", "Shows messages or asks user for input.");
        addDocumentation("settings", "Array settings()", "Returns array with names of all custom options.");
        addDocumentation("settings", "Value settings(optionName)", "Returns value for an option.");
        addDocumentation("settings", "settings(optionName)", "Sets value for a new option or overrides existing option.");
        addDocumentation("dateString", "String dateString(format)", "Returns text representation of current date and time.");
        addDocumentation("networkGet", "NetworkReply networkGet(url)", "Sends HTTP GET request.");
        addDocumentation("networkPost", "NetworkReply networkPost(url, postData)", "Sends HTTP POST request.");
        addDocumentation("env", "ByteArray env(name)", "Returns value of environment variable with given name.");
        addDocumentation("setEnv", "bool setEnv(name, value)", "Sets environment variable with given name to given value.");
        addDocumentation("sleep", "sleep(time)", "Wait for given time in milliseconds.");
        addDocumentation("ByteArray", "ByteArray", "Wrapper for QByteArray Qt class.");
        addDocumentation("File", "File", "Wrapper for QFile Qt class.");
        addDocumentation("Dir", "Dir", "Wrapper for QDir Qt class.");
        addDocumentation("TemporaryFile", "TemporaryFile", "Wrapper for QTemporaryFile Qt class.");
        addDocumentation("arguments", "(Array)", "Array for accessing arguments passed to current function or the script");
        addDocumentation("Item", "(Object)", "Type is `Object` and each property is MIME type with data.");
        addDocumentation("FinishedCommand", "(Object)", "Type is `Object` and properties are:");
        addDocumentation("NetworkReply", "(Object)", "Type is `Object` and properties are:");
        addDocumentation("mimeText", "(text/plain)", "Data contains plain text content.");
        addDocumentation("mimeHtml", "(text/html)", "Data contains HTML content.");
        addDocumentation("mimeUriList", "(text/uri-list)", "Data contains list of links to files, web pages etc.");
        addDocumentation("mimeWindowTitle", "(application/x-copyq-owner-window-title)", "Current window title for copied clipboard.");
        addDocumentation("mimeItems", "(application/x-copyq-item)", "Serialized items.");
        addDocumentation("mimeItemNotes", "(application/x-copyq-item-notes)", "Data contains notes for item.");
        addDocumentation("mimeOwner", "(application/x-copyq-owner)", "If available, the clipboard was set from CopyQ (from script or copied items).");
        addDocumentation("mimeClipboardMode", "(application/x-copyq-clipboard-mode)", "Contains `selection` if data is from X11 mouse selection.");
        addDocumentation("mimeCurrentTab", "(application/x-copyq-current-tab)", "Current tab name when invoking command from main window.");
        addDocumentation("mimeSelectedItems", "(application/x-copyq-selected-items)", "Selected items when invoking command from main window.");
        addDocumentation("mimeCurrentItem", "(application/x-copyq-current-item)", "Current item when invoking command from main window.");
        addDocumentation("mimeHidden", "(application/x-copyq-hidden)", "If set to `1`, the clipboard or item content will be hidden in GUI.");
        addDocumentation("mimeShortcut", "(application/x-copyq-shortcut)", "Application or global shortcut which activated the command.");
        addDocumentation("mimeColor", "(application/x-copyq-color)", "Item color (same as the one used by themes).");
        addDocumentation("mimeOutputTab", "(application/x-copyq-output-tab)", "Name of the tab where to store new item.");
        addDocumentation("mimeSyncToClipboard", "(application/x-copyq-sync-to-selection)", "If exists the X11 selection data will be copied to clipboard.");
        addDocumentation("mimeSyncToSelection", "(application/x-copyq-sync-to-clipboard)", "If exists the clipboard data will be copied to X11 selection.");
    }

    void addDocumentation(const QString &name, const QString &api, const QString &documentation)
    {
        auto &doc = m_doc[name].doc;
        if (!doc.isEmpty())
            doc.append("\n");

        doc.append(api + "\n    " + documentation);
    }

    ScriptableDocumentation documentationForRow(int row) const
    {
        const auto index2 = this->index(row, 0);
        const auto text = QStringListModel::data(index2, Qt::EditRole).toString();
        return m_doc.value(text);
    }

    QString typeForRow(int row) const
    {
        const auto tagText = documentationForRow(row).tag;
        if (tagText.isEmpty())
            return QString();

        const char tag = tagText[0].toLatin1();
        switch (tag) {
        case 'a': return "array";
        case 'k': return "keyword";
        case 'f': return "function";
        case 'o': return "object";
        case 't': return "type";
        default: return QString();
        }
    }

    QHash<QString, ScriptableDocumentation> m_doc;
};

void setUpHeader(QHeaderView *header)
{
    header->hide();
#if QT_VERSION < 0x050000
    header->setResizeMode(QHeaderView::ResizeToContents);
#else
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif
}

class CommandCompleterPopup : public QTableView {
public:
    explicit CommandCompleterPopup(QWidget *parent)
        : QTableView(parent)
    {
        setUpHeader(horizontalHeader());
        setUpHeader(verticalHeader());
        setShowGrid(false);
        setContentsMargins(0, 0, 0, 0);
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setAlternatingRowColors(true);
    }

protected:
    void showEvent(QShowEvent *event) override
    {
        QTableView::showEvent(event);
        startTimer(0);
    }

    void timerEvent(QTimerEvent *event) override
    {
        QTableView::timerEvent(event);

        if (!model())
            return;

        const auto margins = contentsMargins();
        const int w = columnsWidth()
                + (verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0)
                + margins.left() + margins.right();
        const int h = rowsHeight(8)
                + (horizontalScrollBar()->isVisible() ? verticalScrollBar()->height() : 0)
                + margins.top() + margins.bottom();
        resize(w, h);
    }

private:
    int columnsWidth() const
    {
        int width = 0;
        for ( int column = 0; column < model()->columnCount(); ++column )
            width += columnWidth(column);
        return width;
    }

    int rowsHeight(int maxRows) const
    {
        int height = 0;
        for ( int row = 0; row < qMin(maxRows, model()->rowCount()); ++row )
            height += rowHeight(row);
        return height;
    }
};

} // namespace

CommandCompleter::CommandCompleter(QPlainTextEdit *editor)
    : QObject(editor)
    , m_editor(editor)
    , m_completer(new QCompleter(new CommandCompleterModel(this), this))
{
    m_completer->setWidget(m_editor);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);

    m_completer->setPopup(new CommandCompleterPopup(m_editor));
    m_completer->popup()->installEventFilter(this);

    connect( m_completer, SIGNAL(activated(QString)),
             this, SLOT(insertCompletion(QString)) );

    connect( m_editor, SIGNAL(textChanged()),
             this, SLOT(updateCompletion()) );
    connect( m_editor, SIGNAL(cursorPositionChanged()),
             m_completer->popup(), SLOT(hide()) );

    auto shortcut = new QShortcut(tr("Ctrl+Space", "Shortcut to show completion menu"), editor);
    connect( shortcut, SIGNAL(activated()),
             this, SLOT(showCompletion()) );
}

bool CommandCompleter::eventFilter(QObject *, QEvent *event)
{
    if (event->type() != QEvent::KeyPress)
        return false;

    QKeyEvent *e = static_cast<QKeyEvent*>(event);
    QAbstractItemView *popup = m_completer->popup();

    switch (e->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
        m_completer->setCurrentRow( popup->currentIndex().row() );
        insertCompletion( m_completer->currentCompletion() );
        popup->hide();
        return true;

    case Qt::Key_Escape:
        popup->hide();
        return true;

    default:
        return false;
    }
}

void CommandCompleter::updateCompletion(bool forceShow)
{
    const QString completionPrefix = textUnderCursor();

    QAbstractItemView *popup = m_completer->popup();

    if ( !forceShow && completionPrefix.length() < 3 ) {
        popup->hide();
    } else {
        if (completionPrefix != m_completer->completionPrefix()) {
            m_completer->setCompletionPrefix(completionPrefix);
            popup->setCurrentIndex(m_completer->completionModel()->index(0, 0));
        }

        const auto rect = m_editor->cursorRect();
        m_completer->complete(rect);
    }
}

void CommandCompleter::insertCompletion(const QString &completion)
{
    QTextCursor tc = m_editor->textCursor();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::StartOfWord);
    tc.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    tc.insertText(completion);
    m_editor->setTextCursor(tc);
}

void CommandCompleter::showCompletion()
{
    updateCompletion(true);
}

QString CommandCompleter::textUnderCursor() const
{
    auto tc = m_editor->textCursor();
    const auto c = tc.document()->characterAt(tc.position() - 1);
    if (!c.isLetter())
        return QString();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}
