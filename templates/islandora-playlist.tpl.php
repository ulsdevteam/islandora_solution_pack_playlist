<?php

/**
 * @file
 * This is the template file for the object page for large image
 *
 * Available variables:
 * - $islandora_object: The Islandora object rendered in this template file
 * - $compound_object_content: A rendered view that would be the same as
 *   whatever object's template would create.
 * - $metadata: This is initially populated with the metadata for the initial
 *   object, but would be updated via javascript when any of the related "child"
 *   audio/video/oral histories objects are played.
 * - $playlist: populated by the module code based on the objects that are
 *   related to the "parent" object by the isConstituentOf relationship.
 *
 * @see template_preprocess_islandora_playlist()
 * @see theme_islandora_playlist()
 */
?>
<?php print $compound_object_content; ?>
<div id="amplitude-player">
   <div class="row">
      <div id="amplitude-left">
         <div id="player-left-bottom">
            <div id="time-container">
               <span class="current-time">
               <span class="amplitude-current-minutes" amplitude-main-current-minutes="true"></span>:<span class="amplitude-current-seconds" amplitude-main-current-seconds="true"></span>
               </span>
               <input type="range" class="amplitude-song-slider" amplitude-main-song-slider="true"/>
               <span class="duration">
               <span class="amplitude-duration-minutes" amplitude-main-duration-minutes="true"></span>:<span class="amplitude-duration-seconds" amplitude-main-duration-seconds="true"></span>
               </span>
            </div>
            <div id="control-container">
               <div id="repeat-container">
                  <div class="amplitude-repeat" id="repeat"></div>
               </div>
               <div id="central-control-container">
                  <div id="central-controls">
                     <div class="amplitude-prev" id="previous"></div>
                     <div class="amplitude-play-pause" amplitude-main-play-pause="true" id="play-pause"></div>
                     <div class="amplitude-next" id="next"></div>
                  </div>
               </div>
            </div>
            <div id="meta-container">
               <span amplitude-song-info="name" amplitude-main-song-info="true" class="song-name"></span>
            </div>
         </div>
      </div>
      <div class="playlist_objects_wrapper" id="amplitude-right">
         <h3>Playlist</h3>
         <?php print $playlist; ?>
      </div>
      <div id="current_metadata"></div>
   </div>
</div>


<div id="playlist_metadata">
  <h3>"<?php print $islandora_object->label; ?>" Metadata</h3>
  <?php print $parent_metadata; ?>
</div>

