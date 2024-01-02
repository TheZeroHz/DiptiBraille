URLStream url(ssid, password);
StreamCopy copier;
File DownloaderFile;
String download_url;
String download_loc;
String download_filename;
class downloader{
public:
void Url(String address);
void FileName(String name);
void SaveTo(String loc);
void Download();
};

void downloader::Url(String address){
  download_url=address;
}
void downloader::FileName(String name){
  download_filename=name;
}
void downloader::SaveTo(String loc){
  download_loc=loc;
}
void downloader::Download(){
url.begin(download_url.c_str());
download_loc+=download_filename;
DownloaderFile = SD.open(download_loc.c_str(), FILE_WRITE);
DownloaderFile.seek(0); // overwrite from beginning
copier.begin(DownloaderFile, url);
copier.copyAll();
DownloaderFile.close();
}
