#include "StyleSheet.h"

QPalette StyleSheet::CreatePaletteLigth()
{
	QPalette paletteLight;
	paletteLight.setColor(QPalette::Window, QColor(240, 240, 240));
	paletteLight.setColor(QPalette::WindowText, Qt::black);
	paletteLight.setColor(QPalette::Base, QColor(255, 255, 255));
	paletteLight.setColor(QPalette::AlternateBase, QColor(233, 231, 227));
	paletteLight.setColor(QPalette::ToolTipBase, Qt::white);
	paletteLight.setColor(QPalette::ToolTipText, Qt::black);
	paletteLight.setColor(QPalette::Text, Qt::black);
	paletteLight.setColor(QPalette::Button, QColor(240, 240, 240));
	paletteLight.setColor(QPalette::ButtonText, Qt::black);
	paletteLight.setColor(QPalette::BrightText, Qt::red);
	paletteLight.setColor(QPalette::Link, QColor(0, 0, 255));
	paletteLight.setColor(QPalette::Highlight, QColor(0, 120, 215));
	paletteLight.setColor(QPalette::HighlightedText, Qt::white);
	return paletteLight;
}

QPalette StyleSheet::CreatePaletteDark()
{
	QPalette paletteDark;
	paletteDark.setColor(QPalette::Window, QColor(53, 53, 53));
	paletteDark.setColor(QPalette::WindowText, Qt::white);
	paletteDark.setColor(QPalette::Base, QColor(25, 25, 25));
	paletteDark.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	paletteDark.setColor(QPalette::ToolTipBase, Qt::black);
	paletteDark.setColor(QPalette::ToolTipText, Qt::white);
	paletteDark.setColor(QPalette::Text, Qt::white);
	paletteDark.setColor(QPalette::Button, QColor(53, 53, 53));
	paletteDark.setColor(QPalette::ButtonText, Qt::white);
	paletteDark.setColor(QPalette::BrightText, Qt::red);
	paletteDark.setColor(QPalette::Link, QColor(42, 130, 218));
	paletteDark.setColor(QPalette::Highlight, QColor(42, 130, 218));
	paletteDark.setColor(QPalette::HighlightedText, Qt::black);
	return paletteDark;
}
