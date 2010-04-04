<?php
// $Id: nitobe_masthead_image.admin.inc,v 1.1.2.1 2010/04/04 21:44:42 pl2 Exp $

/**
 * @file
 * Administration pages for the nitobe masthead module.
 */

/**
 * Main config page.
 *
 * Prints a list of paths already configured.
 */
function nitobe_masthead_image_admin_build_nitobemasthead() {
  $output = '';

  $result = db_query("SELECT * FROM {nitobe_masthead_image}");

  $headers = array(t('Path'), t('Image'), array('data' => t('Ops'), 'colspan' => 2));
  while ($row = db_fetch_object($result)) {
    $rows[] = array(
      check_plain($row->path),
      check_plain($row->image),
      l(t('Edit'), 'admin/build/nitobemasthead/'. $row->id .'/edit'),
      l(t('Delete'), 'admin/build/nitobemasthead/'. $row->id .'/delete'),
    );
  }

  $output .= theme('table', $headers, $rows);

  return $output;
}

/**
 * Add/edit form for setting an image on a specified path.
 */
function nitobe_masthead_image_admin_build_nitobemasthead_add($form_state, $item = NULL) {

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $item ? check_plain($item->path) : '',
    '#description' => t('Use &lt;front&gt; for the front page.'),
  );

  require_once(drupal_get_path('theme', 'nitobe') .'/nitobe_utils.inc');

  $images  = _nitobe_get_header_list(TRUE);
  $options = array();

  foreach ($images as $filename => $data) {
    $options[basename($filename)] = $data->pretty_name;
  }

  $form['image'] = array(
    '#type' => 'select',
    '#title' => t('Image'),
    '#options' => $options,
    '#default_value' => $item ? $item->image : '',
  );

  if ($item) {
    $form['id'] = array(
      '#type' => 'value',
      '#value' => $item->id,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for the add/edit form.
 */
function nitobe_masthead_image_admin_build_nitobemasthead_add_submit($form, &$form_state) {
  $item = $form['#parameters'][2];

  if ($item) {
    drupal_write_record('nitobe_masthead_image', $form_state['values'], 'id');
  }
  else {
    drupal_write_record('nitobe_masthead_image', $form_state['values']);
  }

  cache_clear_all();

  $form_state['redirect'] = 'admin/build/nitobemasthead';
}

/**
 * Delete a path/image relationship item form.
 */
function nitobe_masthead_image_admin_build_nitobemasthead_delete($form_state, $item) {
  return confirm_form($form, t('Are you sure that you want to delete this item?'), 'admin/build/nitobemasthead');
}

/**
 * Submit handler for the delete form.
 */
function nitobe_masthead_image_admin_build_nitobemasthead_delete_submit($form, &$form_state) {
  $item = $form['#parameters'][2];

  if ($item) {
    db_query("DELETE FROM {nitobe_masthead_image} WHERE id = %d", $item->id);
  }

  cache_clear_all();

  $form_state['redirect'] = 'admin/build/nitobemasthead';
}
