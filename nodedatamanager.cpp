#include "nodedatamanager.h"

NodeDataManager::NodeDataManager()
{

}

QString NodeDataManager::createNodeXml(NodeProperties *nodeProperties)
{
    QString nodeXml;
    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(&nodeXml);
    xmlWriter->writeEmptyElement("Node");
    xmlWriter->writeAttribute("id",QString::number(nodeProperties->nodeID));
    xmlWriter->writeAttribute("text", nodeProperties->nodeText);
    xmlWriter->writeTextElement("x", QString::number(nodeProperties->x));
    xmlWriter->writeTextElement("y", QString::number(nodeProperties->y));
    xmlWriter->writeEndDocument();
    qInfo(nodeXml.toUtf8());
    return nodeXml;
}
