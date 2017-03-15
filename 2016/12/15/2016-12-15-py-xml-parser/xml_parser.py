from xml.dom import minidom

xmldoc = minidom.parse('items.xml')
#itemlist = xmldoc.getElementsByTagName('Access')
accessList = xmldoc.getElementsByTagName('AccessList')

print(len(accessList))
for s in accessList:
    itemList = s.getElementsByTagName('Access')
    print(itemList)
    for e in itemList:
        print(e.firstChild.data)


accessList = xmldoc.getElementsByTagName('LTEBandList')

print(len(accessList))
for s in accessList:
    itemList = s.getElementsByTagName('LTEBand')
    print(itemList)
    for e in itemList:
        name = e.getElementsByTagName('Name')[0]
        value = e.getElementsByTagName('Value')[0]
        
        print("name = " + str(name.firstChild.data))
        print("value = " + str(value.firstChild.data))


