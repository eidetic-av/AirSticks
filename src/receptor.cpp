#include "receptor.hpp"

#include <iterator>

template <typename Out>
void split(const std::string& s, char delim, Out result) {
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

struct Receptor : Module {
	enum ParamIds { NUM_PARAMS };
	enum OutputIds {
		LEFT_TRIGGER_OUTPUT,
		RIGHT_TRIGGER_OUTPUT,
		LEFT_BUMPER_OUTPUT,
		RIGHT_BUMPER_OUTPUT,
		LEFT_JOYSTICK_X_OUTPUT,
		RIGHT_JOYSTICK_X_OUTPUT,
		LEFT_JOYSTICK_Y_OUTPUT,
		RIGHT_JOYSTICK_Y_OUTPUT,
		LEFT_BUTTON_3_OUTPUT,
		LEFT_BUTTON_4_OUTPUT,
		RIGHT_BUTTON_4_OUTPUT,
		RIGHT_BUTTON_3_OUTPUT,
		LEFT_BUTTON_1_OUTPUT,
		LEFT_BUTTON_2_OUTPUT,
		RIGHT_BUTTON_2_OUTPUT,
		RIGHT_BUTTON_1_OUTPUT,
		LEFT_POSITION_Z_OUTPUT,
		LEFT_POSITION_X_OUTPUT,
		LEFT_POSITION_Y_OUTPUT,
		RIGHT_POSITION_X_OUTPUT,
		RIGHT_POSITION_Z_OUTPUT,
		RIGHT_POSITION_Y_OUTPUT,
		LEFT_ROTATION_Z_OUTPUT,
		LEFT_ROTATION_X_OUTPUT,
		LEFT_ROTATION_Y_OUTPUT,
		RIGHT_ROTATION_X_OUTPUT,
		RIGHT_ROTATION_Z_OUTPUT,
		RIGHT_ROTATION_Y_OUTPUT,
		NUM_OUTPUTS
	};
	enum InputIds { NUM_INPUTS };
	enum LightIds { NUM_LIGHTS };

	static std::vector<Receptor*> instances;
	static bool threadRunning;

	Receptor() { config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS); }

	void onAdd() override {
		if (!threadRunning) {
			std::thread receptorThread([] {
					osc::AirSticksListener listener;
					UdpListeningReceiveSocket socket(
							IpEndpointName(IpEndpointName::ANY_ADDRESS, 12812),
							&listener);
					socket.Run();
					});
			receptorThread.detach();
			threadRunning = true;
		}
		instances.push_back(this);
	}
	void process(const ProcessArgs& args) override {}
};

bool Receptor::threadRunning = false;
std::vector<Receptor*> Receptor::instances;

void osc::AirSticksListener::ProcessMessage(
		const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint) {
	osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
	std::vector<std::string> address = split(m.AddressPattern(), '/');

	if (std::strcmp(address[1].c_str(), "airsticks") == 0) {
		bool leftHand = true;
		if (std::strcmp(address[2].c_str(), "right") == 0) leftHand = false;

		for (auto& receptor : Receptor::instances) {
			osc::ReceivedMessage::const_iterator args = m.ArgumentsBegin();

			if (std::strcmp(address[3].c_str(), "buttons") == 0) {
				if (std::strcmp(address[4].c_str(), "pressed") == 0) {
					receptor->outputs[Receptor::RIGHT_BUTTON_1_OUTPUT]
						.setVoltage((args++)->AsBool() ? 10 : 0);
					receptor->outputs[Receptor::RIGHT_BUTTON_2_OUTPUT]
						.setVoltage((args++)->AsBool() ? 10 : 0);
					receptor->outputs[Receptor::RIGHT_BUTTON_3_OUTPUT]
						.setVoltage((args++)->AsBool() ? 10 : 0);
					receptor->outputs[Receptor::RIGHT_BUTTON_4_OUTPUT]
						.setVoltage((args++)->AsBool() ? 10 : 0);
					receptor->outputs[Receptor::RIGHT_TRIGGER_OUTPUT]
						.setVoltage((args++)->AsBool() ? 10 : 0);
					receptor->outputs[Receptor::RIGHT_BUMPER_OUTPUT].setVoltage(
							(args++)->AsBool() ? 10 : 0);
				}
			}
			m.ArgumentsEnd();
		}
	}
}

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

		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(24.18, 16.529)), module, Receptor::LEFT_TRIGGER_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(58.134, 16.61)), module, Receptor::RIGHT_TRIGGER_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(10.247, 20.144)), module, Receptor::LEFT_BUMPER_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(72.067, 20.226)), module, Receptor::RIGHT_BUMPER_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(10.225, 39.126)), module,
					Receptor::LEFT_JOYSTICK_X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(72.089, 39.208)), module,
					Receptor::RIGHT_JOYSTICK_X_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(24.13, 43.995)), module,
					Receptor::LEFT_JOYSTICK_Y_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(58.184, 44.077)), module,
					Receptor::RIGHT_JOYSTICK_Y_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(9.537, 61.466)), module, Receptor::LEFT_BUTTON_3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(24.13, 61.466)), module, Receptor::LEFT_BUTTON_4_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(72.777, 61.548)), module,
					Receptor::RIGHT_BUTTON_4_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(58.184, 61.548)), module,
					Receptor::RIGHT_BUTTON_3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(11.79, 77.536)), module, Receptor::LEFT_BUTTON_1_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(22.303, 77.536)), module,
					Receptor::LEFT_BUTTON_2_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(70.525, 77.618)), module,
					Receptor::RIGHT_BUTTON_2_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(60.011, 77.618)), module,
					Receptor::RIGHT_BUTTON_1_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(32.716, 95.659)), module,
					Receptor::LEFT_POSITION_Z_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(11.815, 95.684)), module,
					Receptor::LEFT_POSITION_X_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(22.253, 95.734)), module,
					Receptor::LEFT_POSITION_Y_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(49.599, 95.741)), module,
					Receptor::RIGHT_POSITION_X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(70.5, 95.766)), module,
					Receptor::RIGHT_POSITION_Z_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(60.062, 95.816)), module,
					Receptor::RIGHT_POSITION_Y_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(32.678, 112.618)), module,
					Receptor::LEFT_ROTATION_Z_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(11.777, 112.643)), module,
					Receptor::LEFT_ROTATION_X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(22.215, 112.693)), module,
					Receptor::LEFT_ROTATION_Y_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(49.636, 112.7)), module,
					Receptor::RIGHT_ROTATION_X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(70.537, 112.725)), module,
					Receptor::RIGHT_ROTATION_Z_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(60.099, 112.775)), module,
					Receptor::RIGHT_ROTATION_Y_OUTPUT));
	}
};

Model* modelReceptor = createModel<Receptor, ReceptorWidget>("Receptor");
