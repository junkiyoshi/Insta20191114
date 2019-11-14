#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
	this->mesh.clear();

	int hue = ofGetFrameNum() * 0.1;
	for (auto x = -300; x <= 300; x += 600) {

		for (auto y = -300; y <= 300; y += 600) {

			auto location = glm::vec3(x, y, 0);
			hue += 64;

			for (auto i = 0; i < 3; i++) {

				auto radius = 250 + i;
				auto noise_base = glm::vec3(ofRandom(-1000, 1000), ofRandom(-1000, 1000), 0);

				ofColor color;
				color.setHsb(int(hue + i * 85) % 255, 180, 255);

				for (auto deg = 0; deg < 360; deg += 1) {

					int index = this->mesh.getVertices().size();
					vector<glm::vec3> vertices = {
						glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 3),
						glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), 3),
						glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), -3),
						glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), -3)
					};

					for (auto& vertex : vertices) {

						auto noise_seed_x = noise_base.x + vertex.x * 0.001;
						auto noise_seed_y = noise_base.y + vertex.y * 0.001;
						auto noise_seed_z = noise_base.z + vertex.z * 0.001;

						auto angle_x = ofMap(ofNoise(noise_seed_x, ofGetFrameNum() * 0.0008), 0, 1, -PI * 2, PI * 2);
						auto angle_y = ofMap(ofNoise(noise_seed_y, ofGetFrameNum() * 0.0008), 0, 1, -PI * 2, PI * 2);
						auto angle_z = ofMap(ofNoise(noise_seed_z, ofGetFrameNum() * 0.0008), 0, 1, -PI * 2, PI * 2);

						auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
						auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
						auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

						this->mesh.addVertex(location + glm::vec4(vertex, 0) * rotation_z * rotation_y * rotation_x);
						this->mesh.addColor(color);
					}

					this->mesh.addIndex(index + 0); this->mesh.addIndex(index + 1); this->mesh.addIndex(index + 2);
					this->mesh.addIndex(index + 0); this->mesh.addIndex(index + 3); this->mesh.addIndex(index + 2);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	
	this->mesh.draw();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}