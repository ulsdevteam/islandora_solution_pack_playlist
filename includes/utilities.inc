<?php
/**
 * @file
 * Utility functions.
 */

/**
 * This will execute the islandora_compound_object function
 * "islandora_compound_object_get_parts" that will return the PID values of all
 * objects that have isConstituentOf relationship to the given (parent)
 * islandora_object.
 *
 * @param type $islandora_object
 * @return array
 */
function islandora_playlist_get_compound_object_children($islandora_object) {
  module_load_include('module', 'islandora_compound_object', 'islandora_compound_object');
  $child_pids = islandora_compound_object_get_parts($islandora_object->id);
  $compound_object_child_playlist_entry = array();
  $objs = '';

  // This will be needed for the full path to the css / js for the player, but this
  // may be removed / replaced.
  $module_path = drupal_get_path('module', 'islandora_playlist');

  foreach ($child_pids as $idx => $child_pid) {
    $child_obj = islandora_object_load($child_pid);
    if (is_object($child_obj)) {
      // Call function that will make the HTML markup to render each playlist
      // entry that corresponds to the object.
      $compound_object_child_playlist_entry[$child_pid] = islandora_playlist_get_playlist_entry_markup($idx, $child_obj, $module_path);

      $objs[] = $child_obj;
    }
  }
  return array('compound_object_child_playlist_entry' => $compound_object_child_playlist_entry,
    'child_islandora_objects' => is_array($objs) ? $objs : FALSE);
}

/** This will inspect the islandora object passed as $child_obj and based on the
 *  model, it will call the appropriate theme template to render each playlist
 *  entry's markup.
 *
 * @param string $idx
 * @param object $child_obj
 * @param string $module_path
 * @return string
 *   markup html for each playlist entry
 */
function islandora_playlist_get_playlist_entry_markup($idx, $child_obj, $module_path) {
  if (isset($child_obj['MP3'])) {
    $url = '/islandora/object/' . $child_obj->id . '/datastream/MP3/view';
  }
  elseif (isset($child_obj['PROXY_MP3'])) {
    $url = '/islandora/object/' . $child_obj->id . '/datastream/PROXY_MP3/view';
  }

  $link_path = "javascript:play_object('" . $child_obj->id . "', '" . $idx . "')";

  return theme('islandora_playlist_playlist_entry_markup',
      array('idx' => $idx,
            'module_path' => $module_path,
            'child_obj' => $child_obj,
            'get_metadata_link' => $link_path,
            'multimedia_url' => $url));
}

/**
 * Depending on which multimedia modules are installed, this will read their
 * configuration variable values to return the default view for each multimedia
 * content type.
 * 
 * @param array $child_islandora_objects
 *   each array element is an islandora object
 * @return string
 *   represents the default multimedia player for the child_islandora_objects.
 */
function islandora_playlist_get_multimedia_player($parent_object, $child_islandora_objects) {
  $models = array();
  $this_model = '';
  if (is_array($child_islandora_objects)) {
    foreach ($child_islandora_objects as $islandora_object) {
      $this_obj_models = $islandora_object->models;
      if (($key = array_search('fedora-system:FedoraObject-3.0', $this_obj_models) !== FALSE)) {
        unset($this_obj_models[$key]);
        $this_model = array_pop($this_obj_models);
        $models[$this_model] = $this_model;
      }
    }
  }
  if (count($models) > 0) {
    watchdog('islandora_playlist', t('This playlist compound object contains '
            . 'more than one type of multimedia file.  Please limit a playlist'
            . 'to only one multimedia content type. ') .
            l('Manage Compound Object', 'islandora/object/' . $parent_object->id . '/manage/compound'));
  }

  $default_viewers = array();
  if (module_exists('islandora_audio')) {
    $islandora_audio_viewers = variable_get('islandora_audio_viewers');
    $default_viewers['slandora:sp-audioCModel'] = $islandora_audio_viewers['default'];
  }
  if (module_exists('islandora_video')) {
    $islandora_video_viewers = variable_get('islandora_video_viewers');
    $default_viewers['islandora:sp_videoCModel'] = $islandora_video_viewers['default'];
    if (module_exists('islandora_oralhistories')) {
      // Since this modules is dependent on the islandora_video module, it will
      // use the same viewer that is configured for videos.
      $default_viewers['islandora:oralhistoriesCModel'] = $islandora_video_viewers['default'];
    }
  }
  if (!isset($default_viewers[$this_model])) {
    watchdog('islandora_playlist', t('There is no default viewer configured for ' .
            'the compound object childrens model of @this_model - ') .
            l('Manage object', 'islandora/object/' . $parent_object->id . '/manage'),
            array('@this_model' => $this_model));
  }
  return $default_viewers;
}

/**
 * This will create the javascript block that represents the playlist that is needed by the player.
 *
 * @param type $compound_object_child_playlist_entry
 */
function islandora_playlist_get_compound_object_children_js_block($child_islandora_objects) {
  $tracks_markup = array();
  if (is_array($child_islandora_objects) && (count($child_islandora_objects) > 0)) {
    foreach ($child_islandora_objects as $islandora_object) {
      if (isset($islandora_object['MP3'])) {
        $url = '/islandora/object/' . $islandora_object->id . '/datastream/MP3/view';
      }
      elseif (isset($islandora_object['PROXY_MP3'])) {
        $url = '/islandora/object/' . $islandora_object->id . '/datastream/PROXY_MP3/view';
      }
      else {
        $url = '';
      }
      $tracks_markup[] = '{ ' .
          '"name": "' . htmlspecialchars($islandora_object->label) . '",' .
          '"url": "' . $url . '",' .
          '}';
    }
    if (count($tracks_markup) > 0) {
      return 'jQuery(document).ready(function () {
  Amplitude.init({
                          "songs": [' .
        implode(",\n", $tracks_markup) .
      ']
    });
  Amplitude.play;
});';
    }
  }
}

/**
 * This will add an entry to the array that represents the islandora object's
 * model.  It will add an entry with 'model' as the key and the value will be 
 * the islandora object's models after removing the FedoraObject-3.0 reference.
 * 
 * @param type $array
 * @param type $islandora_object
 * @return array
 */
function islandora_playlist_insert_obj_model_to_array($array, $islandora_object,
  $insert_key_name, $before_key) {
  $this_obj_models = $islandora_object->models;
  if (($key = array_search('fedora-system:FedoraObject-3.0', $this_obj_models) !== FALSE)) {
    unset($this_obj_models[$key]);
  }
  $ret_array = array();
  $before_key_last_value = 1;
  foreach ($array as $key => $value) {
    if ($key <> $before_key) {
      $ret_array[$key] = $value;
    }
    else {
      $before_key_last_value = $value;
    }
  }
  $insert_value = implode(', ', $this_obj_models);
  $ret_array[$insert_key_name] = $insert_value;
  // Now, add the previous value back at the end of the array.
  $ret_array[$before_key] = $before_key_last_value;

  return $ret_array;
}
