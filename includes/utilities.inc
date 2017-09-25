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
  $default_viewers = array();
  if (module_exists('islandora_audio')) {
    $islandora_audio_viewers = variable_get('islandora_audio_viewers');
    $default_viewers['islandora:sp-audioCModel'] = $islandora_audio_viewers['default'];
  }
  if (module_exists('islandora_video')) {
    $islandora_video_viewers = variable_get('islandora_video_viewers');
    $default_viewers['islandora:sp_videoCModel'] = $islandora_video_viewers['default'];
    if (module_exists('islandora_oralhistories')) {
      // Since this modules is dependent on the islandora_video module, it uses
      // the same viewer that is configured for videos.
      $default_viewers['islandora:oralhistoriesCModel'] = $islandora_video_viewers['default'];
    }
  }

  $models = array();
  if (count($child_islandora_objects) > 0) {
    foreach ($child_islandora_objects as $islandora_object) {
      $this_obj_models = $islandora_object->models;
      if (is_array($this_obj_models)) {
        foreach ($this_obj_models as $model) {
          if ($model <> 'fedora-system:FedoraObject-3.0') {
            $models[$model] = $model;
          }
        }
      }
    }
  }
  if (count($models) > 1) {
    watchdog('islandora_playlist', t('This playlist compound object contains '
            . 'more than one type of multimedia file.  Please limit a playlist'
            . 'to only one multimedia content type. ') .
            l('Manage Compound Object', 'islandora/object/' . $parent_object->id . '/manage/compound'));
  }

  $key_model = array_pop($models);
  if (!isset($default_viewers[$key_model])) {
    watchdog('islandora_playlist', t('There is no default viewer configured for ' .
            'the compound object childrens model of @this_model - ') .
            l('Manage object', 'islandora/object/' . $parent_object->id . '/manage'),
            array('@this_model' => $key_model));
    return FALSE;
  }

  // Based on the $default_viewers[$key_model], the javascript and CSS that
  // needs to be included will be different.
  if ($key_model == 'islandora:sp-audioCModel') {
    drupal_add_js(drupal_get_path('module', 'islandora_playlist') . '/js/play_object.js');
    drupal_add_css(drupal_get_path('module', 'islandora_playlist') . '/css/islandora_playlist.theme.css');

    // These javascript and CSS are needed for the amplitude player...
    // TODO: replace this
    drupal_add_js(drupal_get_path('module', 'islandora_playlist') . '/js/functions.js');
    drupal_add_js('sites/all/libraries/amplitudejs/dist/amplitude.js');
    drupal_add_js('sites/all/libraries/amplitudejs/examples/resources/js/foundation.min.js');
    drupal_add_css(drupal_get_path('module', 'islandora_playlist') . '/css/styles.css');
  }

  return $key_model;
}

/**
 * This will return the URL of the multimedia playback url for the islandora_object
 * based on the model
 */
function islandora_playlist_get_child_multimedia_url($child_islandora_object, $child_model) {
  if ($child_model == 'islandora:sp-audioCModel') {
    if (isset($child_islandora_object['PROXY_MP3'])) {
      $url = '/islandora/object/' . $child_islandora_object->id . '/datastream/PROXY_MP3/view';
    }
    elseif (isset($child_islandora_object['MP3'])) {
      $url = '/islandora/object/' . $child_islandora_object->id . '/datastream/MP3/view';
    }
    elseif (isset($child_islandora_object['OBJ'])) {
      $url = '/islandora/object/' . $child_islandora_object->id . '/datastream/OBJ/view';
    }
    else {
      $url = '';
    }
  }
  elseif ($child_model == 'islandora:sp-videoCModel') {
    if (isset($child_islandora_object['MP4'])) {
      $url = '/islandora/object/' . $child_islandora_object->id . '/datastream/MP4/view';
    }
    elseif (isset($child_islandora_object['AVI'])) {
      $url = '/islandora/object/' . $child_islandora_object->id . '/datastream/AVI/view';
    }
    elseif (isset($child_islandora_object['OBJ'])) {
      $url = '/islandora/object/' . $child_islandora_object->id . '/datastream/OBJ/view';
    }
    else {
      $url = '';
    }
  }
  return $url;
}

/**
 * This will create the javascript block that represents the playlist that is needed by the player.
 *
 * @param type $compound_object_child_playlist_entry
 */
function islandora_playlist_get_compound_object_children_js_block($child_islandora_objects, $child_model) {
  $tracks_markup = array();
  if (is_array($child_islandora_objects) && (count($child_islandora_objects) > 0)) {
    foreach ($child_islandora_objects as $child_islandora_object) {
      $url = islandora_playlist_get_child_multimedia_url($child_islandora_object, $child_model);

      $tracks_markup[] = '{ ' .
          '"name": "' . htmlspecialchars($child_islandora_object->label) . '",' .
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
