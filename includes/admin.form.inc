<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_playlist_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_playlist', 'includes/utilities');
  $get_default_value = function($name, $default) use(&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };
  $form = array();
  $info = theme('item_list', array('items' => array(
        'compound object\'s child Audio, Video, or Oral Histories objects',
        'can not mix content types in a single playlist')));
  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Usage Information'),
    '#description' => t('Playlist constituant objects must all be related using ' .
      'Compound Objects.') . $info,
  );
  $form['islandora_playlist_audio'] = array(
    '#type' => 'fieldset',
    '#title' => t('Islandora Audio is ' . (module_exists('islandora_audio') ? '' : 'not ') . 'installed'),
    '#description' => t('Playlist can support compound constituant objects that are Islandora Audio objects.') .
      (module_exists('islandora_audio') ? '' : '<br>Download from here: <b>' .
            l('Islandora Audio Solution Pack', 'https://github.com/Islandora/islandora_solution_pack_audio') . '</b>.'),
  );
  $form['islandora_playlist_video'] = array(
    '#type' => 'fieldset',
    '#title' => t('Islandora Video is ' . (module_exists('islandora_video') ? '' : 'not ') . 'installed'),
    '#description' => t('Playlist can support compound constituant objects that are Islandora Video objects.') .
      (module_exists('islandora_video') ? '' : '<br>Download from here: <b>' .
            l('Islandora Video Solution Pack', 'https://github.com/Islandora/islandora_solution_pack_video') . '</b>.'),
  );
  $form['islandora_playlist_oralhistories'] = array(
    '#type' => 'fieldset',
    '#title' => t('Islandora Oral Histories is ' . (module_exists('islandora_oralhistories') ? '' : 'not ') . 'installed'),
    '#description' => t('Playlist can support compound constituant objects that are Islandora Oral Histories objects.') .
      (module_exists('islandora_oralhistories') ? '' : '<br>Download from here: <b>' .
            l('Islandora Oral Histories Solution Pack', 'https://github.com/Islandora-Labs/islandora_solution_pack_oralhistories') . '</b>.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submit.
 */
function islandora_playlist_admin_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#id'];
  switch ($op) {
    case 'edit-submit':
      drupal_set_message('Save configuration clicked.  There is nothing to save.');
      break;
  }
}
