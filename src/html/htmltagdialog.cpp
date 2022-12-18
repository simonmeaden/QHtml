#include "SMLibraries/widgets/html/htmltagdialog.h"

#include "SMLibraries/widgets/html/htmltypes.h"
#include "SMLibraries/utilities/characters.h"
#include <SMLibraries/widgets/helpdialogs.h>

HtmlTagDialog::HtmlTagDialog(QWidget* parent)
  : QDialog(parent)
{
  initGui();
}

void
HtmlTagDialog::initGui()
{
  auto layout = new QGridLayout;
  setLayout(layout);

  auto lbl = new QLabel(tr("Tag:"), this);
  layout->addWidget(lbl, 0, 0);

  m_tagEdit = new QLineEdit(this);
  m_tagEdit->setPlaceholderText(tr("Empty tag"));
  layout->addWidget(m_tagEdit, 0, 1);

  lbl = new QLabel(tr("Tag Names:"), this);
  layout->addWidget(lbl, 1, 0, Qt::AlignTop);

  auto tags = HtmlTypes::tagNames();
  m_tagList = new QListWidget(this);
  m_tagList->addItems(tags);
  m_tagList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  layout->addWidget(m_tagList, 1, 1, 4, 1);
  connect(m_tagList,
          &QListWidget::itemDoubleClicked,
          this,
          &HtmlTagDialog::setTagName);

  m_attrLbl = new QLabel(tr("Empty Attribute"), this);
  layout->addWidget(m_attrLbl, 1, 2, 1, 4, Qt::AlignTop);

  lbl = new QLabel(tr("Attribute Names:"), this);
  layout->addWidget(lbl, 2, 2, Qt::AlignTop);

  m_attrList = new QListWidget(this);
  m_attrList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  layout->addWidget(m_attrList, 2, 3 /*, 2, 1*/);
  connect(m_attrList,
          &QListWidget::itemDoubleClicked,
          this,
          &HtmlTagDialog::setAttrName);

  lbl = new QLabel(tr("Attribute Values:"), this);
  layout->addWidget(lbl, 2, 4, Qt::AlignTop);

  m_valueList = new QListWidget(this);
  m_valueList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  layout->addWidget(m_valueList, 2, 5 /*, 2, 1*/);
  connect(m_valueList,
          &QListWidget::itemDoubleClicked,
          this,
          &HtmlTagDialog::setAttrValue);

  auto btn = new QPushButton(tr("Add Attribute"), this);
  layout->addWidget(btn, 4, 2, 1, 4);

  auto btnbox = new QDialogButtonBox(
    QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help);
  btnbox->button(QDialogButtonBox::Ok)->setText(tr("Accept Tag"));
  connect(btnbox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(btnbox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(btnbox, &QDialogButtonBox::rejected, this, &HtmlTagDialog::help);

  layout->addWidget(btnbox, 5, 0, 1, 6);
}

void
HtmlTagDialog::buildTag()
{
  QString tag;
  if (m_tagdata.name.isEmpty())
    return;

  tag = m_tagdata.name;
  if (!m_tagdata.name.isEmpty()) {
    for (auto attribute : m_tagdata.attributes) {
      if (attribute) {
        if (!attribute->name.isEmpty()) {
          tag += Characters::SPACE;
          tag += attribute->name;
        }
        if (!attribute->value.isEmpty()) {
          tag += Characters::EQUALS;
          tag += attribute->value;
        }
      }
    }
  }

  m_tagEdit->setText(tag);
}

void
HtmlTagDialog::buildAttribute()
{
  QString attribute;
  if (!m_attribute)
    return;

  if (m_attribute->name.isEmpty()) {
    m_attrLbl->clear();
    return;
  }

  if (!m_attribute->name.isEmpty()) {
    attribute += Characters::SPACE;
    attribute += m_attribute->name;
  }
  if (!m_attribute->value.isEmpty()) {
    attribute += Characters::EQUALS;
    attribute += m_attribute->value;
  }
}

void
HtmlTagDialog::setTagName(QListWidgetItem* item)
{
  auto text = item->text();
  if (m_tagdata.name.isEmpty()) {
    m_tagdata.name = text;
    m_attrData = HtmlTypes::attributesForTag(text);
    m_attrList->clear();
    m_attrList->addItems(m_attrData.keys());
  } else {
    // TODO an undo facility if changing tag name ???
    if (text != m_tagdata.name) {
      m_tagdata.name = text;
      m_attrData = HtmlTypes::attributesForTag(text);
      m_attrList->clear();
      m_attrList->addItems(m_attrData.keys());
      if (m_attribute) {
        delete m_attribute;
        m_attrLbl->clear();
      }
      m_attribute = new Attribute();
      if (!m_tagdata.attributes.isEmpty()) {
        qDeleteAll(m_tagdata.attributes);
        m_tagdata.attributes.clear();
      }
    }
  }
  buildTag();
}

void
HtmlTagDialog::setAttrName(QListWidgetItem* item)
{
  auto text = item->text();
  if (!m_attribute)
    m_attribute = new Attribute;

  if (m_tagdata.attributes.isEmpty()) {
    m_attribute->name = text;
  } else {
    if (text != m_attribute->name) {
      m_attribute->name = text;
      m_attrLbl->clear();
    }
  }
  auto values = m_attrData.value(text);
  m_valueList->clear();
  m_valueList->addItems(values);
  buildAttribute();
}

void
HtmlTagDialog::setAttrValue(QListWidgetItem* item)
{
  auto text = item->text();
  if (!m_attribute) // should never happen
    m_attribute = new Attribute;
  m_attribute->value = text;
  buildAttribute();
}

void
HtmlTagDialog::addAttribute(const QString& name)
{
  m_tagdata.attributes.append(m_attribute);
  m_attribute = nullptr;
  buildTag();
}

void
HtmlTagDialog::help()
{
  auto dlg = new SimpleHelpDialog(tr("Html Tag Builder"), this);
  dlg->setHelpText(tr("Help text"));
  dlg->show();
}
