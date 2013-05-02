#include <QFileInfo>
#include <QDir>
#include <QVector>
#include <QSimpleXmlNodeModel>

class FileTree : public QSimpleXmlNodeModel
{
public:
  FileTree (const QXmlNamePool &namePool);
  mutable QVector<QFileInfo>  m_fileInfos;

  QXmlNodeModelIndex nodeFor (const QString &fileName) const;


  virtual QXmlNodeModelIndex::DocumentOrder compareOrder (const QXmlNodeModelIndex &, const QXmlNodeModelIndex &) const;
  virtual QXmlName name (const QXmlNodeModelIndex &node) const;
  virtual QUrl documentUri (const QXmlNodeModelIndex &node) const;
  virtual QXmlNodeModelIndex::NodeKind kind (const QXmlNodeModelIndex &node) const;
  virtual QXmlNodeModelIndex root (const QXmlNodeModelIndex &node) const;
  virtual QVariant typedValue (const QXmlNodeModelIndex &node) const;

protected:

  virtual QVector<QXmlNodeModelIndex> attributes (const QXmlNodeModelIndex &element) const;
  virtual QXmlNodeModelIndex nextFromSimpleAxis (SimpleAxis, const QXmlNodeModelIndex &) const;


private:

  enum Type
  {
    File,
    Directory,
    AttributeFileName,
    AttributeFilePath,
    AttributeSize,
    AttributeMIMEType,
    AttributeSuffix
  };


  inline QXmlNodeModelIndex nextSibling (const QXmlNodeModelIndex &nodeIndex,
                                         const QFileInfo &from,
                                         qint8 offset) const;
  inline const QFileInfo &toFileInfo (const QXmlNodeModelIndex &index) const;
  inline QXmlNodeModelIndex toNodeIndex (const QFileInfo &index,
                                         Type attributeName) const;
  inline QXmlNodeModelIndex toNodeIndex (const QFileInfo &index) const;

  /*
   One possible improvement is to use a hash, and use the &*&value()
   trick to get a pointer, which would be stored in data() instead
   of the index.
  */

  const QDir::Filters         m_filterAllowAll;
  const QDir::SortFlags       m_sortFlags;
  QVector<QXmlName>           m_names;

};
