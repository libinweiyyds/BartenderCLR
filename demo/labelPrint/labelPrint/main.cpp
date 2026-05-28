#include "labelPrint.h"
#include <QtWidgets/QApplication>
#include "thirty_part/BartenderCLR/include/BartenderCppAPI.h"
#include "QDebug"
#pragma execution_character_set("UTF-8") 
int main(int argc, char* argv[])
{
	//QApplication app(argc, argv);
	//labelPrint window;
	//window.show();
	void* handle = CreateBartenderAPI();
	qDebug() << handle;

	if (handle == nullptr)
	{
		qDebug() << "创建打印示例失败！";
	}
	else {
		qDebug() << "创建打印示例成功！";
	}

	char* printerNames[100];

	for (int i = 0; i < 100; i++)
	{
		printerNames[i] = new char[256];
	}

	int count = GetPrinterNames(handle, printerNames, 100);

	for (int i = 0; i < count; i++)
	{
		printf("Printer[%d]: %s\n", i, printerNames[i]);
	}

	int res = StartEngine(handle);
	if (res == 1)
	{
		qDebug() << "启动Bartender引擎成功！";
	}
	else {
		qDebug() << "启动Bartender引擎失败";
		GetLastErrorMessage(handle);
	}

	if (OpenLabelFormatDocument(handle, "D:\\Desktop\\T68-G HD集成全景后摄像头\\T68-G HD集成全景后摄像头.btw") == 1) {
		qDebug() << "打开btw文件成功！";
	}
	else {
		qDebug() << "打开btw文件失败！";
		qDebug()<<GetLastErrorMessage(handle);
	}

	const int MAX = 20;
	char** names = (char**)malloc(MAX * sizeof(char*));
	count = GetDocumentSubStrings(handle, names, MAX);

	// 打印
	for (int i = 0; i < count; i++) {
		qDebug() << names[i];
	}

	// 释放内存
	for (int i = 0; i < count; i++) free(names[i]);
	free(names);


	
	try
	{
		SetSubStringValue(handle, "part_no", "this is first code");
	}
	catch (...)
	{
		qDebug() << GetLastErrorMessage(handle);
	}
	char *msg=(char*)malloc(10);
	PrintLabel(handle, "Microsoft Print to PDF", 1000, &msg, 10);





	//return app.exec();
	return 1;
}
