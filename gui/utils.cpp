// MIT License Terms (http://en.wikipedia.org/wiki/MIT_License)
//
// Copyright (C) 2011 by Oleg Alexandrov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#ifdef ADD_PREF_INI
#include <qapplication.h>
#include <qsettings.h>
#include <qdesktopservices.h>
#include <qdir.h>
#endif // ADD_PREF_INI
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <cstring>
#include <algorithm>
#include "utils.h"

using namespace std;
#define DEF_WIDTH 900
#define DEF_HEIGHT 700


#ifdef ADD_PREF_INI
// single INI file
static  QString m_sSettingsFile;
#endif // ADD_PREF_INI

void utils::printUsage(std::string progName) 
{
    const char *ind = "   ";
    cout << endl;
    cout << ind << "Usage: " << progName << " [options] " << "[file_1.xg [ ... file_N.xg]]" << endl;
    cout << endl;
    cout << ind << "options:" << endl;
    cout << ind << " -geo[metry] " << DEF_WIDTH << "x" << DEF_HEIGHT << endl;
    cout << ind << " -backgroundColor black (-bg)" << endl;
    cout << ind << " -color yellow          (-c)" << endl;
    cout << ind << " -fontSize 10           (-fs)" << endl;
    cout << ind << " -lineWidth 2           (-lw)" << endl;
    cout << ind << " -points                (-p)" << endl;
    cout << ind << " -closedPoly            (-cp)" << endl;
    cout << ind << " -nonClosedPoly         (-nc)" << endl;
    cout << ind << " -filledPoly            (-f)" << endl;
    cout << ind << " -nonFilledPoly         (-nf)" << endl;
    cout << ind << " -grid on | off" << endl;
    cout << ind << " -gridSize 10" << endl;
    cout << ind << " -gridWidth 1" << endl;
    cout << ind << " -gridColor green" << endl;
    int nColors = 0;
    const char **colors = utils::getColorTable(&nColors);
    if (colors && (nColors > 1)) {
        int i, cnt, wrap = 4;
        cout << endl;
        cout << ind << "Color can also be set by a numeric index 0 - " << (nColors - 1) << endl;
        cout << ind;
        cnt = 0;
        for (i = 0; i < nColors; i++) {
            cout << i << " " << colors[i] << " ";
            cnt++;
            if (cnt == wrap) {
                cnt = 0;
                cout << endl;
                if ((i + 1) < nColors) {
                    cout << ind;
                }
            }
        }
        if (cnt)
            cout << endl;
        cout << ind << "Larger numeric indexs are wrapped at " << nColors << endl;

    }
}

void utils::extractWindowDims(// inputs
                              int numArgs, char ** args,
                              // outputs
                              int & windowWidX, int & windowWidY
                              ){

  // Parse the command line arguments '-geo[metry] 500x600'

  windowWidX = DEF_WIDTH; windowWidY = DEF_HEIGHT; // defaults

  for (int s = 1; s < numArgs; s++){

    if ( !strstr(args[s-1], "-geo") ) continue;

    string lineStr = args[s];
    char * line    = (char*) lineStr.c_str();

    // Blank the geometry settings once located
    // to not confuse other parsers.
    args[s-1][0] = '\0';
    args[s  ][0] = '\0';

    char * pch;
    char delimiter[] = "x";

    pch = strtok (line, delimiter);
    if (pch == NULL) continue;
    int windowWidX_tmp = atoi(pch);

    pch = strtok (NULL, delimiter);
    if (pch == NULL) continue;
    int windowWidY_tmp = atoi(pch);

    if (windowWidX_tmp > 0 && windowWidY_tmp > 0){
      windowWidX = windowWidX_tmp;
      windowWidY = windowWidY_tmp;
#ifdef ADD_PREF_INI
      writeSetting("width",windowWidX);
      writeSetting("height",windowWidY);
#endif // ADD_PREF_INI

    }
    
    }

  }

#ifdef ADD_PREF_INI
static bool doneINIFile = false;
void utils::setINIFile()
{
    QDir d;
    std::string s;
#ifdef USE_QT4_DEFS
    if (!doneINIFile) {
        // this return the executable path
        //m_sSettingsFile = QApplication::applicationDirPath();
        // this returns 'C:\Users\user'
        //m_sSettingsFile = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
        // this returns 'C:\Users\user\AppData\Roaming\Microsoft\Windows\Start Menu\Programs'
        //m_sSettingsFile = QDesktopServices::storageLocation(QDesktopServices::ApplicationsLocation);
        // this returns 'C:\Users\user\AppData\Local\Polyview\Polyview-2D'
        m_sSettingsFile = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
        d.mkpath(m_sSettingsFile);
        m_sSettingsFile.append(QDir::separator());
        m_sSettingsFile.append("polyview.ini");
    }
#else
    // TODO: Qt5 - set INI file save
    if (!doneINIFile) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Polyview-2D", "Polyview-2D");
        //QString location = QFileInfo(settings.fileName()).absolutePath();
        //s = location.toStdString();
        //m_sSettingsFile = location;
        m_sSettingsFile = QFileInfo(settings.fileName()).absolutePath();
        //m_sSettingsFile = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
        d.mkpath(m_sSettingsFile);
        m_sSettingsFile.append(QDir::separator());
        m_sSettingsFile.append("polyview.ini");
        //s = m_sSettingsFile.toStdString();
        //cout << "INI: '" << s << "'" << endl;
    }
#endif
    doneINIFile = true;
}

int utils::loadSettingInt(const QString &key, int def)
{
    setINIFile();
    QSettings settings(m_sSettingsFile, QSettings::IniFormat);
    return settings.value(key,def).toInt();
}

std::string utils::loadSettingStdString(const QString &key, std::string def)
{
    setINIFile();
    QSettings settings(m_sSettingsFile, QSettings::IniFormat);
    QString s;
    s = QString::fromStdString(def);
    return settings.value(key,s).toString().toStdString();
}

bool utils::loadSettingBool(const QString &key, bool def )
{
    setINIFile();
    QSettings settings(m_sSettingsFile, QSettings::IniFormat);
    return settings.value(key,def).toBool();
}


void utils::loadSettings(polyOptions &opt,int & windowWidX, int & windowWidY)
{
    QString s;

    setINIFile();

    QSettings settings(m_sSettingsFile, QSettings::IniFormat);
    // proceceed to read settings...

    windowWidX = settings.value("width",DEF_WIDTH).toInt();
    windowWidY = settings.value("height",DEF_HEIGHT).toInt();
    opt.plotAsPoints = settings.value("plotAsPoinsts",opt.plotAsPoints).toBool();
    opt.isPolyFilled = settings.value("isPolyFilled",opt.isPolyFilled).toBool();
    opt.isPolyClosed = (closedPolyInfo) settings.value("isPolyClosed",opt.isPolyClosed).toInt();
    // bool useCmdLineColor;
    opt.fontSize = settings.value("fontSize",opt.fontSize).toInt();
    opt.lineWidth = settings.value("lineWidth",opt.lineWidth).toInt();
    opt.isGridOn = settings.value("isGridOn",opt.isGridOn).toBool();
    opt.gridSize = settings.value("gridSize",opt.gridSize).toDouble();
    opt.gridWidth = settings.value("gridWidth",opt.gridWidth).toInt();
    opt.isLatLonOn = settings.value("isLatLonOn",opt.isLatLonOn).toBool();
    s = QString::fromStdString(opt.bgColor);
    opt.bgColor = settings.value("bgColor",s).toString().toStdString();
    s = QString::fromStdString(opt.fgColor);
    opt.fgColor = settings.value("fgColor",s).toString().toStdString();
    s = QString::fromStdString(opt.cmdLineColor);
    opt.cmdLineColor = settings.value("cmdLineColor",s).toString().toStdString();
    s = QString::fromStdString(opt.gridColor);
    opt.gridColor = settings.value("gridColor",s).toString().toStdString();
    s = QString::fromStdString(opt.latlonColor);
    opt.latlonColor = settings.value("latlonColor",s).toString().toStdString();
    s = QString::fromStdString(opt.polyFileName);
    opt.polyFileName = settings.value("polyFileName",s).toString().toStdString();

    // m_useNmScale = settings.value("useNmScale",0);

}

void utils::writeSetting(const QString &key, const QVariant &variant)
{
    QSettings settings(m_sSettingsFile, QSettings::IniFormat);
    settings.setValue(key, variant);
}

#endif // ADD_PREF_INI


void utils::parseCmdOptions(//inputs
                            int argc, char** argv, std::string exeName,
                            // outputs
                            int & windowWidX, int & windowWidY, cmdLineOptions & options
                            ){

  options.polyOptionsVec.clear();

  polyOptions opt; // Each polygon file will have one such entry

#ifdef ADD_PREF_INI
  // also sets the INI file name
  loadSettings(opt,windowWidX,windowWidY);
#endif // ADD_PREF_INI

  // Skip argv[0] as that's the program name
  extractWindowDims(argc - 1, argv + 1, windowWidX, windowWidY);

  for (int argIter = 1; argIter < argc; argIter++){

    char * currArg = argv[argIter];

    if (currArg == NULL || strlen(currArg) == 0) continue;

    if (currArg[0] == '-'){
      // Transform -P into -p, etc.
      transform(currArg, currArg + strlen(currArg), currArg, ::tolower);
    }

    if (strcmp( currArg, "-h"     ) == 0 || strcmp( currArg, "--h"    ) == 0 ||
        strcmp( currArg, "-help"  ) == 0 || strcmp( currArg, "--help" ) == 0 ||
        strcmp( currArg, "-?"     ) == 0 || strcmp( currArg, "--?"    ) == 0 ){
      printUsage(exeName);
      //exit(0);
      continue;
    }

    if ( strcmp(currArg, "-p") == 0 || strcmp(currArg, "-points") == 0 ){
      opt.plotAsPoints = !opt.plotAsPoints;
#ifdef ADD_PREF_INI
      writeSetting("plotAsPoints",opt.plotAsPoints);
#endif // ADD_PREF_INI
      continue;
    }

    if ( strcmp(currArg, "-f") == 0 || strcmp(currArg, "-filledpoly") == 0 ){
      opt.isPolyFilled = true;
#ifdef ADD_PREF_INI
      writeSetting("isPolyFilled",true);
#endif // ADD_PREF_INI
      continue;
    }

    if ( strcmp(currArg, "-nf") == 0 || strcmp(currArg, "-nonfilledpoly") == 0 ){
      opt.isPolyFilled = false;
#ifdef ADD_PREF_INI
      writeSetting("isPolyFilled",false);
#endif // ADD_PREF_INI
      continue;
    }

    if ( strcmp(currArg, "-cp") == 0 || strcmp(currArg, "-closedpoly") == 0 ){
      // Plot as closed polygons
      opt.isPolyClosed = forceClosedPoly;
#ifdef ADD_PREF_INI
      writeSetting("isPolyClosed",(int)forceClosedPoly);
#endif // ADD_PREF_INI
      continue;
    }

    if ( strcmp(currArg, "-nc") == 0 || strcmp(currArg, "-nonclosedpoly") == 0 ){
      // Plot as polygonal lines
      opt.isPolyClosed = forceNonClosedPoly;
#ifdef ADD_PREF_INI
      writeSetting("isPolyClosed",(int)forceNonClosedPoly);
#endif // ADD_PREF_INI
      continue;
    }

    if ( (strcmp(currArg, "-bg") == 0 || strcmp(currArg, "-backgroundcolor") == 0 )
         &&
         argIter < argc - 1
         ){
      opt.bgColor = argv[argIter + 1];
#ifdef ADD_PREF_INI
      writeSetting("bgColor",opt.bgColor.c_str());
#endif // ADD_PREF_INI
      argIter++;
      continue;
    }

    if ( (strcmp(currArg, "-fs"      ) == 0 ||
          strcmp(currArg, "-fontsize") == 0 )
         &&
         argIter < argc - 1
         ){
      int fs = (int)round(atof(argv[argIter + 1]));
      if (fs > 0) {
          opt.fontSize = fs;
#ifdef ADD_PREF_INI
          writeSetting("fontSize",fs);
#endif // ADD_PREF_INI
      }
      argIter++;
      continue;
    }

    if ( (strcmp(currArg, "-lw"       ) == 0 ||
          strcmp(currArg, "-linewidth") == 0 )
         &&
         argIter < argc - 1
         ){
      int lw = (int)round(atof(argv[argIter + 1]));
      if (lw > 0) {
          opt.lineWidth = lw;
#ifdef ADD_PREF_INI
          writeSetting("lineWidth",lw);
#endif // ADD_PREF_INI
      }
      argIter++;
      continue;
    }

    if ( strcmp(currArg, "-gridsize") == 0 &&
         argIter < argc - 1
         ){
      double gs = atof(argv[argIter + 1]);
      if (gs > 0) {
          opt.gridSize = gs;
#ifdef ADD_PREF_INI
          writeSetting("gridSize",gs);
#endif // ADD_PREF_INI
      }
      argIter++;
      continue;
    }

    if ( strcmp(currArg, "-gridwidth") == 0  &&
         argIter < argc - 1
         ){
      int gw = (int)round(atof(argv[argIter + 1]));
      if (gw > 0) {
          opt.gridWidth = gw;
#ifdef ADD_PREF_INI
          writeSetting("gridWidth",gw);
#endif // ADD_PREF_INI
      }
      argIter++;
      continue;
    }

    if ( strcmp(currArg, "-gridcolor") == 0  &&
         argIter < argc - 1
         ){
      opt.gridColor = argv[argIter + 1];
#ifdef ADD_PREF_INI
      writeSetting("gridColor",opt.gridColor.c_str());
#endif // ADD_PREF_INI
      argIter++;
      continue;
    }

    if ( strcmp(currArg, "-grid") == 0  &&
         argIter < argc - 1             &&
         strcmp(argv[argIter + 1], "on") == 0
         ){
      opt.isGridOn = true;
#ifdef ADD_PREF_INI
      writeSetting("isGridOn",true);
#endif // ADD_PREF_INI
      argIter++;
      continue;
    }

    if ( (strcmp(currArg, "-c"    ) == 0 ||
          strcmp(currArg, "-color") == 0 )
         && argIter < argc - 1){
      opt.useCmdLineColor = true;
      opt.cmdLineColor    = argv[argIter + 1];
#ifdef ADD_PREF_INI
      writeSetting("cmdLineColor",opt.cmdLineColor.c_str());
#endif // ADD_PREF_INI
      argIter++;
      continue;
    }

    // Other command line options are ignored
    if (currArg[0] == '-') continue;

    opt.polyFileName = currArg;

    options.polyOptionsVec.push_back(opt);
  }

  // Push one more time, to guarantee that the options vector is
  // non-empty even if no polygons were provided as input, and to make
  // sure we also parsed the options after the last polygon filename.
  options.polyOptionsVec.push_back(opt);

  return;
}

std::string utils::inFileToOutFile(const std::string & inFile){

  string outFile = "";

  bool lastDot = true;
  for (int s = (int)inFile.length() - 1; s >= 0; s--){

    string currChar = inFile.substr(s, 1);
    if (currChar == "." && lastDot){
      outFile = string("_out") + currChar + outFile;
      lastDot = false;
    }else{
      outFile = currChar + outFile;
    }

  }

  if (outFile.length() == 0){
    cerr << "Invalid filename" << endl;
  }

  return outFile;

}

std::string utils::getFilenameExtension(std::string filename){

  std::string::size_type idx;
  idx = filename.rfind('.');

  if(idx != std::string::npos) return filename.substr(idx+1);
  else                         return "";
}

std::string utils::replaceAll(std::string result,
                              const std::string & replaceWhat,
                              const std::string & replaceWithWhat){

  while(1){
    const int pos = result.find(replaceWhat);
    if (pos == -1) break;
    result.replace(pos,replaceWhat.size(),replaceWithWhat);
  }
  return result;
}

///////////////////////////////////////////////////////////////
/// from : http://blog.julien.cayzac.name/2008/10/arc-and-distance-between-two-points-on.html
/// @brief The usual PI/180 constant
static const double DEG_TO_RAD = 0.017453292519943295769236907684886;
/// @brief Earth's quatratic mean radius for WGS-84
static const double EARTH_RADIUS_IN_METERS = 6372797.560856;

/** @brief Computes the arc, in radian, between two WGS-84 positions.
  *
  * The result is equal to <code>Distance(from,to)/EARTH_RADIUS_IN_METERS</code>
  *    <code>= 2*asin(sqrt(h(d/EARTH_RADIUS_IN_METERS )))</code>
  *
  * where:<ul>
  *    <li>d is the distance in meters between 'from' and 'to' positions.</li>
  *    <li>h is the haversine function: <code>h(x)=sin²(x/2)</code></li>
  * </ul>
  *
  * The haversine formula gives:
  *    <code>h(d/R) = h(from.lat-to.lat)+h(from.lon-to.lon)+cos(from.lat)*cos(to.lat)</code>
  *
  * @sa http://en.wikipedia.org/wiki/Law_of_haversines
  */

class Position {
public:
    Position(double ilat, double ilon) { lat = ilat; lon = ilon; }
    ~Position() { }
    double lat,lon;
};


double ArcInRadians(const Position& from, const Position& to) {
    double latitudeArc  = (from.lat - to.lat) * DEG_TO_RAD;
    double longitudeArc = (from.lon - to.lon) * DEG_TO_RAD;
    double latitudeH = sin(latitudeArc * 0.5);
    latitudeH *= latitudeH;
    double lontitudeH = sin(longitudeArc * 0.5);
    lontitudeH *= lontitudeH;
    double tmp = cos(from.lat*DEG_TO_RAD) * cos(to.lat*DEG_TO_RAD);
    return 2.0 * asin(sqrt(latitudeH + tmp*lontitudeH));
}

/** @brief Computes the distance, in meters, between two WGS-84 positions.
  *
  * The result is equal to <code>EARTH_RADIUS_IN_METERS*ArcInRadians(from,to)</code>
  *
  * @sa ArcInRadians
  */
double DistanceInMeters(const Position& from, const Position& to) {
    return EARTH_RADIUS_IN_METERS*ArcInRadians(from, to);
}

double DistanceInMeters(double lat1, double lon1, double lat2, double lon2)
{
    Position p1(lat1,lon1);
    Position p2(lat2,lon2);
    return DistanceInMeters( p1, p2 );
}

///////////////////////////////////////////////////////////////

// eof
