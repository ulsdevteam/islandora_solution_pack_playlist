<?php

/**
 * @file
 * This is the template file for the playlist entry for video objects.
 *
 * Available variables:
 *  - $idx,
 *  - $module_path,
 *  - $child_obj,
 *  - $get_metadata_link,
 *  - $multimedia_url
 *
 * @see template_preprocess_islandora_playlist_entry()
 * @see theme_islandora_playlist_entry()
 */
?>

      <div class="song amplitude-song-container amplitude-play-pause" amplitude-song-index="<?php print $idx; ?>">
          <div class="song-now-playing-icon-container">
                <div class="play-button-container">
                </div>
                <img class="now-playing" src="/<?php print $module_path; ?>/img/now-playing.svg"/>
          </div>
          <div class="song-meta-data">
                <span class="song-title"><?php print l( t($child_obj->label), $get_metadata_link,
                  array(
                  'attributes' => array('title' => t('Play ' . $child_obj->label)),
                  'external'=>true)); ?></span>
          </div>
          <a href="<?php print $multimedia_url; ?>" class="bandcamp-link" target="_blank"></a>
      </div>