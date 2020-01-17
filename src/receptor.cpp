#include "plugin.hpp"

struct Receptor : Module {
	enum ParamIds { NUM_PARAMS };
	enum InputIds {
		LEFT_TRIGGER_INPUT,
		RIGHT_TRIGGER_INPUT,
		LEFT_BUMPER_INPUT,
		RIGHT_BUMPER_INPUT,
		LEFT_JOYSTICK_X_INPUT,
		RIGHT_JOYSTICK_X_INPUT,
		LEFT_JOYSTICK_Y_INPUT,
		RIGHT_JOYSTICK_Y_INPUT,
		LEFT_BUTTON_3_INPUT,
		LEFT_BUTTON_4_INPUT,
		RIGHT_BUTTON_4_INPUT,
		RIGHT_BUTTON_3_INPUT,
		LEFT_BUTTON_1_INPUT,
		LEFT_BUTTON_2_INPUT,
		RIGHT_BUTTON_2_INPUT,
		RIGHT_BUTTON_1_INPUT,
		LEFT_POSITION_Z_INPUT,
		LEFT_POSITION_X_INPUT,
		LEFT_POSITION_Y_INPUT,
		RIGHT_POSITION_X_INPUT,
		RIGHT_POSITION_Z_INPUT,
		RIGHT_POSITION_Y_INPUT,
		LEFT_ROTATION_Z_INPUT,
		LEFT_ROTATION_X_INPUT,
		LEFT_ROTATION_Y_INPUT,
		RIGHT_ROTATION_X_INPUT,
		RIGHT_ROTATION_Z_INPUT,
		RIGHT_ROTATION_Y_INPUT,
		NUM_INPUTS
	};
	enum OutputIds { NUM_OUTPUTS };
	enum LightIds { NUM_LIGHTS };

	Receptor() { config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS); }

	void process(const ProcessArgs& args) override {}
};

struct ReceptorWidget : ModuleWidget {
	ReceptorWidget(Receptor* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(
					asset::plugin(pluginInstance, "res/Receptor.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(
					Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(
					Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(
				createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,
						RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(24.18, 16.529)), module, Receptor::LEFT_TRIGGER_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(58.134, 16.61)), module, Receptor::RIGHT_TRIGGER_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(10.247, 20.144)), module, Receptor::LEFT_BUMPER_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(72.067, 20.226)), module, Receptor::RIGHT_BUMPER_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(10.225, 39.126)), module,
					Receptor::LEFT_JOYSTICK_X_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(72.089, 39.208)), module,
					Receptor::RIGHT_JOYSTICK_X_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(24.13, 43.995)), module,
					Receptor::LEFT_JOYSTICK_Y_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(58.184, 44.077)), module,
					Receptor::RIGHT_JOYSTICK_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(9.537, 61.466)), module, Receptor::LEFT_BUTTON_3_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(24.13, 61.466)), module, Receptor::LEFT_BUTTON_4_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(72.777, 61.548)), module,
					Receptor::RIGHT_BUTTON_4_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(58.184, 61.548)), module,
					Receptor::RIGHT_BUTTON_3_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(11.79, 77.536)), module, Receptor::LEFT_BUTTON_1_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(22.303, 77.536)), module, Receptor::LEFT_BUTTON_2_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(70.525, 77.618)), module,
					Receptor::RIGHT_BUTTON_2_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(60.011, 77.618)), module,
					Receptor::RIGHT_BUTTON_1_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(32.716, 95.659)), module,
					Receptor::LEFT_POSITION_Z_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(11.815, 95.684)), module,
					Receptor::LEFT_POSITION_X_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(22.253, 95.734)), module,
					Receptor::LEFT_POSITION_Y_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(49.599, 95.741)), module,
					Receptor::RIGHT_POSITION_X_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(70.5, 95.766)), module,
					Receptor::RIGHT_POSITION_Z_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(60.062, 95.816)), module,
					Receptor::RIGHT_POSITION_Y_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(32.678, 112.618)), module,
					Receptor::LEFT_ROTATION_Z_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(11.777, 112.643)), module,
					Receptor::LEFT_ROTATION_X_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(22.215, 112.693)), module,
					Receptor::LEFT_ROTATION_Y_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(49.636, 112.7)), module,
					Receptor::RIGHT_ROTATION_X_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(70.537, 112.725)), module,
					Receptor::RIGHT_ROTATION_Z_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(60.099, 112.775)), module,
					Receptor::RIGHT_ROTATION_Y_INPUT));
	}
};

Model* modelReceptor = createModel<Receptor, ReceptorWidget>("Receptor");
