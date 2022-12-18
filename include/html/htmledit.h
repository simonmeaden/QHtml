#pragma once

#include <QApplication>
#include <QColorDialog>
#include <QGroupBox>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMimeData>
#include <QPair>
#include <QTableWidget>
#include <QToolTip>
#include <QTextCursor>

#include "html_global.h"
#include "lnplaintext/lnplaintextedit.h"

class HtmlHighlighter;
class HtmlScanner;
class Tag;
class Attribute;
class HtmlEdit;

class HtmlEditSettings : public LNPlainTextEditSettings
{
public:
  HtmlEditSettings(QObject* parent);
  HtmlEditSettings(BaseConfig* config, QObject* parent);

  YAML::Node createNode(YAML::Node root, YAML::Node parent) override;
  bool load() override;
};

class HtmlEditSettingsWidget : public LNPlainTextEditSettingsWidget
{
  Q_OBJECT

  enum Colors
  {
    NoType,
    Text,
    Background,
    Selection,
    SelectionBackground,
    Parenthesis,
    CurrentLineNumberAreaText,
    CurrentLineNumberAreaBackground,
    LineNumberAreaText,
    LineNumberAreaBackground,
    QuotedString,
    ApostrophiedString,
    TagName,
    AttributeName,
    AttributeValue,
    Comment,
    SpecialChars,
    SpecialCharsBackground,
  };

public:
  HtmlEditSettingsWidget(HtmlEditSettings* settings,
                         HtmlHighlighter* highlighter,
                         HtmlEdit* parent);

  // SettingsWidget interface
  bool isModified() const;

protected:
  void initGui(int& row);

private:
  bool m_modified = false;
  HtmlEdit* m_editor;
  HtmlHighlighter* m_highlighter;
  HtmlEdit* m_display = nullptr;
  HtmlEditSettings* m_settings;
  QMap<Colors, QColor> m_colorMap;
  QMap<Colors, QList<QTableWidgetItem*>> m_itemMap;

  void colorChanged(QTableWidgetItem* item);
  void textChanged();
  void backChanged();
  void currLNAreaBackChanged();
  void currLNAreaTextChanged();
  void lnAreaTextChanged();
  void lnAreaBackChanged();
  void specTextChanged();
  void specBackChanged();
  void resetDisplaySize(int size);
};
// Q_DECLARE_METATYPE(HtmlEditSettings::Colors);

enum HtmlButtonType
{
  NoButton = -1,
  TextType,
  CommentType,
  HtmlType,
};
Q_DECLARE_METATYPE(HtmlButtonType)

class HTML_SHARED_EXPORT HtmlEdit : public LNPlainTextEdit
{
  Q_OBJECT
public:
  HtmlEdit(BaseConfig* settings, QWidget* parent);

  // IEPubEditor interface
  QTextCursor currentCursor();
  void setCurrentCursor(const QTextCursor& cursor);

  //! Returns the file name loaded via loadFile(const QString&) or
  //! loadHref(const QString&, const QString&)
  const QString filename() const;
  //! Loads the file in href into the editor.
  void loadFile(const QString& filename);
  //! Loads the file in href from the zipped file zipfile.
  void loadFromZip(const QString& zipFile, const QString& href);
  //! Loads plain text into the editor
  void setText(const QString& text);

  HtmlScanner* scanner() const;

  void rehighlight();

  // LNPlainTextEdit interface
  bool isModified() const override;

  void optionsDialog();

  //  KeyEventMap* keyMap();
  //  void setKeyMap(KeyEventMap* KeyMap);

signals:
  void lostFocus(QWidget*);
  void gotFocus(QWidget*);
  void mouseClicked(QPoint pos);
  void cursorPositionChanged(QTextCursor cursor);

protected:
  void focusInEvent(QFocusEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  //  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  bool eventFilter(QObject* obj, QEvent* event) override;
  void hoverEnter(QPoint pos);
  void hoverLeave();
  void hoverMove(QPoint pos);
  //! \reimplements{lNPlainTextEdit::contextMenuEvent(QContextMenuEvent*)
  void contextMenuEvent(QContextMenuEvent* event) override;

  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void dragLeaveEvent(QDragLeaveEvent* event) override;
  void dropEvent(QDropEvent* event) override;

  void setFontSize(int size);
  void setFontFamily(const QString& family);

private:
  QTextDocument* m_document = nullptr;
  QWidget* m_parent;
  QString m_filename;
  QString m_zipFile;
  HtmlScanner* m_scanner;
  HtmlHighlighter* m_highlighter;
  Tag* m_hoverTag = nullptr;
  Attribute* m_hoverAttribute = nullptr;
  HtmlEditSettings* m_settings;
  QPoint m_dragStartPosition;

  //  void cursorPositionHasChanged();
  void textHasChanged(int position, int charsRemoved, int charsAdded);
  //  void textModified(bool modified);
  void hoverCheckTags(QPoint pos, int cursorPosition);

  friend class HtmlEditSettingsWidget;
  void handleMatching(QTextCursor cursor);

  void cursorPositionHasChanged();

  //  QPair<Tag*, Tag*> createTagPairAt(int position);
};
