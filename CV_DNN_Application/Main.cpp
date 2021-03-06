// CV_DNN_Application.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "AlexNetClassifier.h"
#include "GoogLeNetClassifier.h"
#include "Paths.h"

#include<memory>
#include "UserCommunicator.h"
#include <sys/stat.h>

/* IMPLEMENT ME */
#define USE_GOOGLE_NET 0

using namespace std;
using namespace OpenCVApp;

int main(int argc, char **argv)
{
	unique_ptr<ClassifierBase> dnnClassifier = nullptr;

	try {
		std::string fileName = (argc > 1) ? argv[1] : UserCommunicator::askForFilename();
		cv::Mat image = cv::imread(fileName, cv::IMREAD_COLOR);
		
#if USE_GOOGLE_NET
		dnnClassifier = unique_ptr<ClassifierBase>(new GoogLeNetClassifier());
#else
		dnnClassifier = unique_ptr<ClassifierBase>(new AlexNetClassifier());
#endif			

		/* 1. ニューラルネットワークを形成する。 */
		dnnClassifier->createNeuralNet();

		/* 2. 入力層に画像をセットする。 */
		dnnClassifier->setImage(&image);

		/* 3. 出力層まで順伝播させる。( = 画像分類を行う。) */
		cv::Mat probabilities;
		dnnClassifier->classify(&probabilities);

		/* 結果表示 */
		UserCommunicator::showClassifiedResult(&probabilities, Paths::MODEL_DIR + "\\synset_words.txt", 5);
	}
	catch (cv::Exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}
