# Islandora Playlist Solution Pack [![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_compound.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_compound)

## Introduction

This solution pack is to render a single page to display a set of related multimedia files in a playlist format.  This module depends on the relationships between parent object and child objects using the Islandora Compound Object Solution Pack.  Even though multiple depths of compound object relationships are possible, "grand-children" relationships are not supported.

The parent object can be any Islandora object whatsoever, but for simplicity a Large Image object can be used as the parent.  A Large Image object would have derivatives for the thumbnail and metadata that would render above the playlist;  a template variable `$compound_object_content` is populated with `drupal_render($variables['page']['content']);` during the playlist_preprocess_page routine.  The template file `templates/islandora-playlist.tpl.php` and `templates/islandora-playlist-entry.tpl.php` can both be themed, but caution should be used when overriding either.

**NOTE:** each playlist should contain children that are all of the same multimedia type because there is no mechanism to switch between multimedia players between tracks.
**NOTE:** This module when using the video-js library will conflict with Islandora JW Player.  In order to use this module with video-js, disable the "Islandora JW Player" module.

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

The initial development of this module uses the [amplitudejs](https://github.com/521dimensions/amplitudejs.git) audio library for playback. This library must be present within sites/all/libraries/amplitudejs or there will be javascript errors when trying to render any playlist object page.  This library is currently being replaced to use whichever multimedia player is configured as the viewer for the Audio/Video/Oral Histories objects.

## Configuration

This is largly dependant on the configuration for the Compound Object (admin/islandora/solution_pack_config/compound_object).

**Block**:

This module does not use either of the blocks that the compound object creates.  Those blocks create static html links that redirect to the actual object instead of playing it.

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors
Current maintainers:

* [Brian Gillingham](https://github.com/bgilling)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)