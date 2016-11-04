#!/usr/bin/lua

require'qtcore'
require'qtgui'

local new_MyWidget = function(...)
	local this = QWidget.new(...)
	local quit = QPushButton.new('Quit', this)
	this:setFixedSize(200, 120)
	quit:setGeometry(62, 40, 75, 30)
	quit:setFont(QFont('Times', 18, 75))
	QObject.connect(quit, '2clicked()', QCoreApplication.instance(), '1quit()')
	return this
end

app = QApplication(1 + select('#', ...), {arg[0], ...})

widget = new_MyWidget()
widget:show()

app.exec()


