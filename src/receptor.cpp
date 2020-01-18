#include "receptor.hpp"

#include "dsp/digital.hpp"

struct Receptor : Module {
	enum ParamIds { TRIG_TOGGLE_PARAM, ACTIVE_TOGGLE_PARAM, NUM_PARAMS };
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
		LEFT_BUTTON_START_OUTPUT,
		RIGHT_BUTTON_START_OUTPUT,
		LEFT_BUTTON_JOYSTICK_OUTPUT,
		RIGHT_BUTTON_JOYSTICK_OUTPUT,
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
	enum LightIds { ACTIVE_TOGGLE_LIGHT, NUM_LIGHTS };

	static bool threadRunning;
	static std::vector<float> voltageStore;

	bool moduleActive = true;
	dsp::BooleanTrigger activeTrigger;

	dsp::BooleanTrigger leftButton1Trigger;
	dsp::BooleanTrigger leftButton2Trigger;
	dsp::BooleanTrigger leftButton3Trigger;
	dsp::BooleanTrigger leftButton4Trigger;
	dsp::BooleanTrigger leftButtonStartTrigger;
	dsp::BooleanTrigger leftButtonJoystickTrigger;
	dsp::BooleanTrigger leftBumperTrigger;
	dsp::BooleanTrigger rightButton1Trigger;
	dsp::BooleanTrigger rightButton2Trigger;
	dsp::BooleanTrigger rightButton3Trigger;
	dsp::BooleanTrigger rightButton4Trigger;
	dsp::BooleanTrigger rightButtonStartTrigger;
	dsp::BooleanTrigger rightButtonJoystickTrigger;
	dsp::BooleanTrigger rightBumperTrigger;

	dsp::PulseGenerator leftButton1Pulse;
	dsp::PulseGenerator leftButton2Pulse;
	dsp::PulseGenerator leftButton3Pulse;
	dsp::PulseGenerator leftButton4Pulse;
	dsp::PulseGenerator leftButtonStartPulse;
	dsp::PulseGenerator leftButtonJoystickPulse;
	dsp::PulseGenerator leftBumperPulse;
	dsp::PulseGenerator rightButton1Pulse;
	dsp::PulseGenerator rightButton2Pulse;
	dsp::PulseGenerator rightButton3Pulse;
	dsp::PulseGenerator rightButton4Pulse;
	dsp::PulseGenerator rightButtonStartPulse;
	dsp::PulseGenerator rightButtonJoystickPulse;
	dsp::PulseGenerator rightBumperPulse;

	Receptor() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(TRIG_TOGGLE_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ACTIVE_TOGGLE_PARAM, 0.f, 1.f, 0.f, "");
	}

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
	}
	void process(const ProcessArgs& args) override {
		if (activeTrigger.process(params[ACTIVE_TOGGLE_PARAM].getValue() > 0.f))
			moduleActive ^= true;

		if (moduleActive) {
			lights[ACTIVE_TOGGLE_LIGHT].setBrightness(10.f);

			// if using "gate" setting, just set output voltages
			// of the buttons same way as all values
			if (params[TRIG_TOGGLE_PARAM].getValue() == 0.f) {
				for (unsigned int i = 0; i < NUM_OUTPUTS; i++)
					outputs[i].setVoltage(voltageStore[i]);
			} else {
				// if using the "trigger" setting, make sure
				// buttons only send a pulse when pressed
				for (unsigned int i = 0; i < NUM_OUTPUTS; i++) {
					if (i == LEFT_BUTTON_1_OUTPUT) {
						if (leftButton1Trigger.process(voltageStore[i]))
							leftButton1Pulse.trigger(1e-3f);
						outputs[i].setVoltage(
								leftButton1Pulse.process(args.sampleTime) ? 10.f
								: 0.f);
					} else if (i == LEFT_BUTTON_2_OUTPUT) {
						if (leftButton2Trigger.process(voltageStore[i]))
							leftButton2Pulse.trigger(1e-3f);
						outputs[i].setVoltage(
								leftButton2Pulse.process(args.sampleTime) ? 10.f
								: 0.f);
					} else if (i == LEFT_BUTTON_3_OUTPUT) {
						if (leftButton3Trigger.process(voltageStore[i]))
							leftButton3Pulse.trigger(1e-3f);
						outputs[i].setVoltage(
								leftButton3Pulse.process(args.sampleTime) ? 10.f
								: 0.f);
					} else if (i == LEFT_BUTTON_4_OUTPUT) {
						if (leftButton4Trigger.process(voltageStore[i]))
							leftButton4Pulse.trigger(1e-3f);
						outputs[i].setVoltage(
								leftButton4Pulse.process(args.sampleTime) ? 10.f
								: 0.f);
					} else if (i == LEFT_BUTTON_JOYSTICK_OUTPUT) {
						if (leftButtonJoystickTrigger.process(voltageStore[i]))
							leftButtonJoystickPulse.trigger(1e-3f);
						outputs[i].setVoltage(
								leftButtonJoystickPulse.process(args.sampleTime)
								? 10.f
								: 0.f);
					} else if (i == LEFT_BUMPER_OUTPUT) {
						if (leftBumperTrigger.process(voltageStore[i]))
							leftBumperPulse.trigger(1e-3f);
						outputs[i].setVoltage(
								leftBumperPulse.process(args.sampleTime) ? 10.f
								: 0.f);
					} else if (i == RIGHT_BUTTON_1_OUTPUT) {
						if (rightButton1Trigger.process(voltageStore[i]))
							rightButton1Pulse.trigger(1e-3f);
						outputs[i].setVoltage(
								rightButton1Pulse.process(args.sampleTime) ? 10.f
								: 0.f);
					} else if (i == RIGHT_BUTTON_2_OUTPUT) {
						if (rightButton2Trigger.process(voltageStore[i]))
							rightButton2Pulse.trigger(1e-3f);
						outputs[i].setVoltage(
								rightButton2Pulse.process(args.sampleTime) ? 10.f
								: 0.f);
					} else if (i == RIGHT_BUTTON_3_OUTPUT) {
						if (rightButton3Trigger.process(voltageStore[i]))
							rightButton3Pulse.trigger(1e-3f);
						outputs[i].setVoltage(
								rightButton3Pulse.process(args.sampleTime) ? 10.f
								: 0.f);
					} else if (i == RIGHT_BUTTON_4_OUTPUT) {
						if (rightButton4Trigger.process(voltageStore[i]))
							rightButton4Pulse.trigger(1e-3f);
						outputs[i].setVoltage(
								rightButton4Pulse.process(args.sampleTime) ? 10.f
								: 0.f);
					} else if (i == RIGHT_BUTTON_JOYSTICK_OUTPUT) {
						if (rightButtonJoystickTrigger.process(voltageStore[i]))
							rightButtonJoystickPulse.trigger(1e-3f);
						outputs[i].setVoltage(
								rightButtonJoystickPulse.process(args.sampleTime)
								? 10.f
								: 0.f);
					} else if (i == RIGHT_BUMPER_OUTPUT) {
						if (rightBumperTrigger.process(voltageStore[i]))
							rightBumperPulse.trigger(1e-3f);
						outputs[i].setVoltage(
								rightBumperPulse.process(args.sampleTime) ? 10.f
								: 0.f);
					} else {
						outputs[i].setVoltage(voltageStore[i]);
					}
				}
			}
		} else
			lights[ACTIVE_TOGGLE_LIGHT].setBrightness(0.f);
	}
};

bool Receptor::threadRunning = false;
std::vector<float> Receptor::voltageStore(Receptor::NUM_OUTPUTS, 0.f);

void osc::AirSticksListener::ProcessMessage(
		const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint) {
	std::vector<std::string> address = split(m.AddressPattern(), '/');

	if (std::strcmp(address[1].c_str(), "airsticks") == 0) {
		bool leftHand = true;
		if (std::strcmp(address[2].c_str(), "right") == 0) leftHand = false;

		osc::ReceivedMessage::const_iterator args = m.ArgumentsBegin();

		if (std::strcmp(address[3].c_str(), "buttons") == 0) {
			if (std::strcmp(address[4].c_str(), "pressed") == 0) {
				if (leftHand) {
					Receptor::voltageStore[Receptor::LEFT_BUTTON_1_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore[Receptor::LEFT_BUTTON_2_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore[Receptor::LEFT_BUTTON_3_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore[Receptor::LEFT_BUTTON_4_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore[Receptor::LEFT_BUTTON_START_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore
						[Receptor::LEFT_BUTTON_JOYSTICK_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore[Receptor::LEFT_BUMPER_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
				} else {
					Receptor::voltageStore[Receptor::RIGHT_BUTTON_1_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore[Receptor::RIGHT_BUTTON_2_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore[Receptor::RIGHT_BUTTON_3_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore[Receptor::RIGHT_BUTTON_4_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore
						[Receptor::RIGHT_BUTTON_START_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore
						[Receptor::RIGHT_BUTTON_JOYSTICK_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
					Receptor::voltageStore[Receptor::RIGHT_BUMPER_OUTPUT] =
						(args++)->AsBool() ? 10.f : 0.f;
				}
			}
		} else if (std::strcmp(address[3].c_str(), "trigger") == 0) {
			if (leftHand)
				Receptor::voltageStore[Receptor::LEFT_TRIGGER_OUTPUT] =
					(args++)->AsFloat() * 10.f;
			else
				Receptor::voltageStore[Receptor::RIGHT_TRIGGER_OUTPUT] =
					(args++)->AsFloat() * 10.f;
		} else if (std::strcmp(address[3].c_str(), "joystick") == 0) {
			if (leftHand) {
				Receptor::voltageStore[Receptor::LEFT_JOYSTICK_X_OUTPUT] =
					(args++)->AsFloat() * 5.f;
				Receptor::voltageStore[Receptor::LEFT_JOYSTICK_Y_OUTPUT] =
					(args++)->AsFloat() * 5.f;
			} else {
				Receptor::voltageStore[Receptor::RIGHT_JOYSTICK_X_OUTPUT] =
					(args++)->AsFloat() * 5.f;
				Receptor::voltageStore[Receptor::RIGHT_JOYSTICK_Y_OUTPUT] =
					(args++)->AsFloat() * 5.f;
			}
		} else if (std::strcmp(address[3].c_str(), "pos") == 0) {
			if (leftHand) {
				Receptor::voltageStore[Receptor::LEFT_POSITION_X_OUTPUT] =
					(args++)->AsFloat() * 5.f;
				Receptor::voltageStore[Receptor::LEFT_POSITION_Y_OUTPUT] =
					(args++)->AsFloat() * 5.f;
				Receptor::voltageStore[Receptor::LEFT_POSITION_Z_OUTPUT] =
					(args++)->AsFloat() * 5.f;
			} else {
				Receptor::voltageStore[Receptor::RIGHT_POSITION_X_OUTPUT] =
					(args++)->AsFloat() * 5.f;
				Receptor::voltageStore[Receptor::RIGHT_POSITION_Y_OUTPUT] =
					(args++)->AsFloat() * 5.f;
				Receptor::voltageStore[Receptor::RIGHT_POSITION_Z_OUTPUT] =
					(args++)->AsFloat() * 5.f;
			}
		} else if (std::strcmp(address[3].c_str(), "angles") == 0) {
			if (leftHand) {
				Receptor::voltageStore[Receptor::LEFT_ROTATION_X_OUTPUT] =
					(args++)->AsFloat() * 5.f;
				Receptor::voltageStore[Receptor::LEFT_ROTATION_Y_OUTPUT] =
					(args++)->AsFloat() * 5.f;
				Receptor::voltageStore[Receptor::LEFT_ROTATION_Z_OUTPUT] =
					(args++)->AsFloat() * 5.f;
			} else {
				Receptor::voltageStore[Receptor::RIGHT_ROTATION_X_OUTPUT] =
					(args++)->AsFloat() * 5.f;
				Receptor::voltageStore[Receptor::RIGHT_ROTATION_Y_OUTPUT] =
					(args++)->AsFloat() * 5.f;
				Receptor::voltageStore[Receptor::RIGHT_ROTATION_Z_OUTPUT] =
					(args++)->AsFloat() * 5.f;
			}
		}
		m.ArgumentsEnd();
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

		addParam(createParamCentered<CKSS>(mm2px(Vec(41.134, 46.195)), module,
					Receptor::TRIG_TOGGLE_PARAM));

		addParam(createParamCentered<LEDBezel>(
					mm2px(Vec(41.134, 67.62)), module, Receptor::ACTIVE_TOGGLE_PARAM));
		addChild(createLightCentered<LEDBezelLight<GreenLight>>(
					mm2px(Vec(41.134, 67.62)), module, Receptor::ACTIVE_TOGGLE_LIGHT));

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
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(26.827, 61.466)), module,
					Receptor::LEFT_BUTTON_4_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(72.777, 61.548)), module,
					Receptor::RIGHT_BUTTON_4_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(55.512, 61.548)), module,
					Receptor::RIGHT_BUTTON_3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(11.79, 77.536)), module, Receptor::LEFT_BUTTON_1_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(23.865, 77.536)), module,
					Receptor::LEFT_BUTTON_2_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(70.525, 77.618)), module,
					Receptor::RIGHT_BUTTON_2_OUTPUT));
		addOutput(
				createOutputCentered<PJ301MPort>(mm2px(Vec(58.474, 77.618)), module,
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
