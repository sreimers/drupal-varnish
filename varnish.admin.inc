<?php
// $Id:

/**
 * @file varnish.admin.inc
 * Administrative functions for Varnish integration.
 */
 
 
/**
* Menu callback for varnish admin settings.
*/
function varnish_admin_settings_form() {
  $form = array();
  
  if (!extension_loaded('sockets')) {
    drupal_set_message(t('Sockets extension not enabled. Varnish terminal communication aborted.'), 'error');
    return $form;
  }
  $form['varnish_control_terminal_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Varnish Control Terminal Server'),
    '#default_value' => variable_get('varnish_control_terminal_server', '127.0.0.1'),
    '#required' => TRUE,    
    '#description' => t('Set this to the server IP or hostname that varnish runs on. This must be configured for Drupal to talk to Varnish.'),
  );
  $form['varnish_control_terminal_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Varnish Control Terminal Server'),
    '#default_value' => variable_get('varnish_control_terminal_port', '8081'),
    '#required' => TRUE,
    '#description' => t('Set this to the port on which your varnish control terminal runs. This must be configured for Drupal to talk to Varnish.'),
  );
  $form['varnish_control_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Varnish Control Key'),
    '#default_value' => variable_get('varnish_control_key', ''),
    '#description' => t('Optional: if you have established a secret key for control terminal access, please put it here.'),
  );
  $form['varnish_cache_clear'] = array(
    '#type' => 'radios',
    '#title' => t('Varnish Cache Clearing'),
    '#options' => array(
      VARNISH_DEFAULT_CLEAR => t('Drupal Default'), 
      VARNISH_SELECTIVE_CLEAR => t('Selective'), 
      VARNISH_NO_CLEAR => t('None'),
    ),
    '#default_value' => variable_get('varnish_cache_clear', VARNISH_DEFAULT_CLEAR),
    '#description' => t('What kind of cache clearing Varnish should utilize. Drupal default will clear all page caches on node updates and cache flush events. Selective will clear a list of URLs you specify as well as any node urls themselves. None will allow stale pages to persist.'),
  );
  $form['varnish_cache_clear_urls'] = array(
    '#type' => 'textarea',
    '#title' => t('Varnish Selective Cache Clear Urls'),
    '#default_value' => variable_get('varnish_cache_clear_urls', '<front>'),
    '#description' => t('One url per line to clear in addition to obvious node-related paths. VCL Regular expressions are permitted.'),
  );
  
  $form['varnish_stats'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Stats'),
  );
  if ($_GET['stats'] == 1) {  
    $status = _varnish_terminal_run('stats', FALSE);
  }
  else {
    $status = l(t('Fetch stats'), 'admin/settings/varnish', array('query' => 'stats=1'));
    $form['varnish_stats']['#collapsed'] = TRUE;
  }
  $form['varnish_stats']['data'] = array(
    '#type' => 'markup',
    '#prefix' => '<pre>',
    '#suffic' => '</pre>',
    '#value' => $status,
  );
  
  return system_settings_form($form);
}


/**
* Menu callback for varnish admin settings.
*/
function varnish_admin_reports_page() {
  // connect to varnish and do a full status report
}