#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <ncurses.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "colors.h"

extern const char *__progname;

typedef struct _ENTRY {
  std::string rev, author, date, msg;
} ENTRY;

static ENTRY currEntry;
static size_t maxRevLen = 0, maxAuthorLen = 0, gap = 1;
static std::vector<ENTRY> entries;
static bool firstLogEntry = true;
static bool showFullMsg = false;
static bool leftAlignMsgs = false;

static void addLogEntry()
{
  currEntry.date[10] = ' ';
  currEntry.date.erase(19);
  entries.push_back(currEntry);

  if (currEntry.rev.length() > maxRevLen)
    maxRevLen = currEntry.rev.length();
  if (currEntry.author.length() > maxAuthorLen)
    maxAuthorLen = currEntry.author.length();
}

static void printLogEntries()
{
  for (auto& e : entries) {
    size_t diff = ((maxRevLen - e.rev.length()) + gap);
    const std::string revSpacer(diff, ' ');

    diff = ((maxAuthorLen - e.author.length()) + gap);
    const std::string authorSpacer((leftAlignMsgs ? diff : gap), ' ');
    const std::string dateSpacer(gap, ' ');
    
    if (!showFullMsg) {
      size_t frontLen = e.rev.length()    + revSpacer.length()  +
                        e.date.length()   + dateSpacer.length() +
                        e.author.length() + authorSpacer.length();
      size_t maxMsgLen = (COLS - frontLen);
      if (e.msg.length() > maxMsgLen) {
        e.msg.erase(maxMsgLen - 3);
        e.msg += "...";
      }
      std::cout << colorCodes[static_cast<int>(Color::byellow)] << e.rev    << revSpacer
                << colorCodes[static_cast<int>(Color::bcyan)]   << e.date   << dateSpacer
                << colorCodes[static_cast<int>(Color::bred)]    << e.author << authorSpacer
                << colorCodes[static_cast<int>(Color::bgreen)]  << e.msg    << std::endl;
    }
    else {
      std::cout << colorCodes[static_cast<int>(Color::byellow)] << e.rev    << revSpacer
                << colorCodes[static_cast<int>(Color::bcyan)]   << e.date   << dateSpacer
                << colorCodes[static_cast<int>(Color::bred)]    << e.author << std::endl
                << colorCodes[static_cast<int>(Color::bgreen)]  << e.msg    << std::endl
                << std::endl;
    }
  }
}

static void traverse(xmlNodePtr node)
{
  xmlChar *txt = nullptr;
  for (xmlNodePtr currNode = node; currNode; currNode = currNode->next) {
    if (currNode->type == XML_ELEMENT_NODE) {
      if (xmlStrEqual(currNode->name, BAD_CAST "logentry")) {
        if (!firstLogEntry)
          addLogEntry();
        else
          firstLogEntry = false;

        txt = xmlGetProp(currNode, BAD_CAST "revision");
        if (txt) {
          currEntry.rev = (char *)txt;
          xmlFree(txt);
          txt = nullptr;
          currEntry.rev.insert(currEntry.rev.begin(), 'r');
        }
      }
      else if (xmlStrEqual(currNode->name, BAD_CAST "author")) {
        txt = xmlNodeGetContent(currNode->children);
        if (txt) {
          currEntry.author = (char *)txt;
          xmlFree(txt);
          txt = nullptr;
        }
      }
      else if (xmlStrEqual(currNode->name, BAD_CAST "date")) {
        txt = xmlNodeGetContent(currNode->children);
        if (txt) {
          currEntry.date = (char *)txt;
          xmlFree(txt);
          txt = nullptr;
        }
      }
      else if (xmlStrEqual(currNode->name, BAD_CAST "msg")) {
        txt = xmlNodeGetContent(currNode->children);
        if (txt) {
          currEntry.msg = (char *)txt;
          xmlFree(txt);
          txt = nullptr;
        }
      }
    }
    traverse(currNode->children);
  }
}

static void loadXML(std::string const& xml)
{
  xmlDocPtr doc = xmlReadMemory(xml.c_str(), xml.length(), "noname.xml", nullptr, 0);
  if (doc == nullptr) {
    std::cerr << "xmlReadMemory() failed to parse document." << std::endl;
  }
  else {
    xmlNodePtr root = xmlDocGetRootElement(doc);
    traverse(root);
    root = nullptr;
    addLogEntry();
    printLogEntries();

    xmlFreeDoc(doc);
    doc = nullptr;
  }
}

static void usage()
{
  std::cout << "usage: "  << __progname << " [OPTIONS]" << std::endl
            << "Options:" << std::endl
            << "  -h               print this message."         << std::endl
            << "  -f               show full commit messages."  << std::endl
            << "  -g <num spaces>  the gap between columns."    << std::endl
            << "  -l               left align commit messages." << std::endl;
}

int main(__attribute__((unused)) int argc,
         __attribute__((unused)) char *argv[])
{
  LIBXML_TEST_VERSION

  int opt = -1;
  while ((opt = getopt(argc, argv, "fg:hl")) != -1) {
    switch (opt) {
    case 'f':
      showFullMsg = true;
      break;
    case 'g':
      gap = atoi(optarg);
      break;
    case 'h':
      usage();
      return EXIT_SUCCESS;
      break;
    case 'l':
      leftAlignMsgs = true;
      break;
    default:
      std::cerr << "Unknown option -" << (char)opt << std::endl;
      return EXIT_FAILURE;
      break;
    }
  }

  // this is merely to get the number of columns (COLS) the term window has
  initscr();
  endwin();

  std::string line, xml;
  while (std::getline(std::cin, line))
    xml += line;

  if (!xml.empty())
    loadXML(xml);

  xmlCleanupParser();
  return EXIT_SUCCESS;
}
