#include "ML_modules.hpp"
#include "dsp/digital.hpp"

#include <sstream>
#include <iomanip>

struct VoltMeter : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};

	VoltMeter() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) { for(int i=0; i<4; i++) volts[i] = 0.0f; };

	void step() override;

	float volts[4];

};



void VoltMeter::step() {

	for(int i=0; i<4; i++) {
		volts[i] = 0.9 * volts[i] + 0.1 * inputs[IN1_INPUT+i].normalize(0.0);
	};



};

struct VoltDisplayWidget : TransparentWidget {

  float  *value;
  std::shared_ptr<Font> font;

  VoltDisplayWidget() {
    font = Font::load(assetPlugin(plugin, "res/Segment7Standard.ttf"));
  };

  void draw(NVGcontext *vg) {
    // Background
    NVGcolor backgroundColor = nvgRGB(0x44, 0x44, 0x44);
    NVGcolor borderColor = nvgRGB(0x10, 0x10, 0x10);
    nvgBeginPath(vg);
    nvgRoundedRect(vg, 0.0, 0.0, box.size.x, box.size.y, 4.0);
    nvgFillColor(vg, backgroundColor);
    nvgFill(vg);
    nvgStrokeWidth(vg, 1.0);
    nvgStrokeColor(vg, borderColor);
    nvgStroke(vg);

    nvgFontSize(vg, 18);
    nvgFontFaceId(vg, font->handle);
    nvgTextLetterSpacing(vg, 2.5);

    char display_string[10];

    sprintf(display_string,"%6.2f",*value);

    Vec textPos = Vec(6.0f, 17.0f);

    NVGcolor textColor = nvgRGB(0xdf, 0xd2, 0x2c);
    nvgFillColor(vg, nvgTransRGBA(textColor, 16));
    nvgText(vg, textPos.x, textPos.y, "~~~", NULL);

    textColor = nvgRGB(0xda, 0xe9, 0x29);
    nvgFillColor(vg, nvgTransRGBA(textColor, 16));
    nvgText(vg, textPos.x, textPos.y, "\\\\\\", NULL);

    textColor = nvgRGB(0xf0, 0x00, 0x00);
    nvgFillColor(vg, textColor);
    nvgText(vg, textPos.x, textPos.y, display_string, NULL);
  }
};


VoltMeterWidget::VoltMeterWidget() {
	VoltMeter *module = new VoltMeter();
	setModule(module);
	box.size = Vec(15*8, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin,"res/VoltMeter.svg")));
		addChild(panel);
	}


	

	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));


	for(int i=0; i<4; i++) {

		addInput(createInput<PJ301MPort>(Vec(12, 60+i*65), module, VoltMeter::IN1_INPUT+i));


		VoltDisplayWidget *display = new VoltDisplayWidget();
		display->box.pos = Vec(10,90+i*65);
		display->box.size = Vec(100, 20);
		display->value = &module->volts[i];
		addChild(display);

	};
	

}
