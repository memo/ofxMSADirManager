
#pragma once

//#include "MSADataProtector.h"

namespace msa {
	
	template <typename T> class DirManager {
	public:
		ofDirectory DIR;

        // hack, to avoid error when not using MSADataProtectorb
        void checkFileMD5(string s, string key, bool check) {return true;}

		void setup(string path, const char *ext = NULL, string* md5 = NULL) {
			currentIndex = 0;
			if(ext) DIR.allowExt(ext);
			int numFiles = DIR.listDir(path);
			
			ofLog(OF_LOG_VERBOSE, "DirManager::setup( " + path + " ) | " + ofToString(numFiles) + " files loaded");
			for(int i = 0; i < numFiles; i++) {
				if(md5) {
					checkFileMD5(DIR.getPath(i), md5[i], true);
				} else {
					checkFileMD5(DIR.getPath(i), "", false);
				}
				string filename = DIR.getPath(i);
				ofLog(OF_LOG_VERBOSE, "   loading " + filename);
				resources.push_back(T());
				loadResource(filename);
			}
		}
		
		virtual void loadResource(string filename) {
		}
		
		
		T &getCurrentFast() {
			return resources[currentIndex];
		}
		
		
		T &getCurrent() {
			if(resources.size() > 0) {
				currentIndex = (currentIndex + resources.size()) % resources.size();
				return resources[currentIndex];
			} else {
				printf("*** No resources yet ***\n");
			}
		}
		
		T &getRandom() {
			currentIndex = rand() % resources.size();
			return getCurrent();
		}
		
		T &getNext() {
			currentIndex++;
			return getCurrent();
		}
		
		T &getPrev() {
			currentIndex--;
			return getCurrent();
		}
		
		T &getAt(int i) {
			currentIndex = i;
			return getCurrent();
		}
		
		int size() {
			return resources.size();
		}
		
	protected:
		vector<T>		resources;
		int				currentIndex;
	};
	
	
	
	//---------------------------
	class ImageDirManager : public DirManager<ofImage*> {
	public:
		void loadResource(string filename) {
			ofImage *s = resources.back() = new ofImage();
			s->loadImage(filename);
//			resources.back().load(filename);
		}
	};
	
	//---------------------------
	class SoundDirManager : public DirManager<ofSoundPlayer*> {
	public:
		void loadResource(string filename) {
			ofSoundPlayer *s = resources.back() = new ofSoundPlayer();
			s->load(filename);
		}
	};
	
	class MovieDirManager : public DirManager<ofVideoPlayer*> {
	public:
		void loadResource(string filename) {
			ofVideoPlayer *s = resources.back() = new ofVideoPlayer();
			s->load(filename);
		}
	};
	
}
