#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

class HtmlTagDialog : public QDialog
{
  Q_OBJECT
  struct Attribute
  {
    QString name;
    QString value;
  };

  struct TagData
  {
    QString name;
    QVector<Attribute*> attributes;
  };

public:
  enum Result
  {
    NoResult,
    BeforeTag,
    AfterTag,
  };
  HtmlTagDialog(QWidget* parent = nullptr);

  Result result() { return m_result; }

private:
  QLineEdit* m_tagEdit;
  QLabel* m_attrLbl;
  QListWidget* m_tagList;
  QListWidget* m_attrList;
  QListWidget* m_valueList;
  Result m_result = NoResult;
  TagData m_tagdata;
  Attribute* m_attribute = nullptr;
  QMap<QString, QVector<QString>> m_attrData;

  void initGui();
  void buildTag();
  void buildAttribute();
  void setTagName(QListWidgetItem *item);
  void setAttrName(QListWidgetItem *item);
  void setAttrValue(QListWidgetItem *item);
  void addAttribute(const QString& name);
  void help();
};

