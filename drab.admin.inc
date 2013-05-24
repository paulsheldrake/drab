<?php

function drab_bundle_form() {
  $form = array();
  
  // All themes
  $themes = list_themes(); 
  
  $default_theme = variable_get('theme_default');
  
  // Create fieldsets of all the themes 
  foreach($themes as $theme){
    $theme_index =  $theme->name; // make sure it's unique
    $theme_name = $theme->info['name'];
    
    // Make everything closed accept the default theme
    $collapsed = TRUE;
    if($default_theme == $theme_index){
      $collapsed = FALSE;
    }
    
    $form[$theme_index] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => $collapsed,
      '#title' => t($theme_name),
    );
    $form[$theme_index]['css'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#title' => t('CSS'),
    );
    $form[$theme_index]['css']['css_files'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#title' => t('Theme CSS files'),
    );
    
    $form[$theme_index]['css']['css_files'][$theme_index . '_list'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Theme CSS files'),
      '#options' => $theme->stylesheets['all'],
    );
    
    $form[$theme_index]['css']['bundles'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#title' => t('Bundles'),
    );
    $form[$theme_index]['css']['bundles']['create'] = array(
      '#type' => 'fieldset',
      '#title' => t('Create/Edit a new bundle'),
      '#description' => t('Check the files from the list above to add to a new 
        bundle or load an existing bundle and update it')
    );
    $form[$theme_index]['css']['bundles']['create'][$theme_index . '_css_bundle_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Bundle name'),
    );
    $form[$theme_index]['css']['bundles']['create'][$theme_index . '_css_bundle_paths'] = array(
      '#type' => 'textarea',
      '#title' => t('Bundle paths'),
      '#description' => t('Paths that the bundle should be applied too.')
    );
    $form[$theme_index]['css']['bundles']['create'][$theme_index . '_create_new_css_bundle'] = array(
      '#type' => 'button',
      '#value' => t('Create a new bundle'),
      '#name' => $theme_index . '_new_bundle',
      '#submit' => array('drab_bundle_form_submit'),
      '#ajax' => array(
        'callback' => 'drab_create_bundle',
      ),
    );
    $form[$theme_index]['css']['bundles']['existing'] = array(
      '#type' => 'fieldset',
      '#title' => t('Existing bundles'),
    );
    
    $bundle_index = $theme_index . '_drab_css_bundles';
    $theme_css_bundles_data = variable_get($bundle_index);
    
    if(!empty($theme_css_bundles_data)){
      $bundles_markup = _drab_generate_existing_bundles_table($theme_css_bundles_data, $theme_index);
    } else {
      $bundles_markup = t('No bundles exist');
    }
    
    
    $bundles_markup_wrapper = theme('html_tag', array('element' => array(
      '#tag' => 'div',
      '#value' => $bundles_markup,
      '#attributes' => array('id' => $bundle_index),
    )));
    
    $form[$theme_index]['css']['bundles']['existing'][$bundle_index] = array(
      '#markup' => $bundles_markup_wrapper,
      
    );
    
  }
  
  //dsm($themes);
  
  return $form;
}


/**
 * Ajax callback function to create a bundle
 */
function drab_create_bundle($form, &$form_state){
 
 /**
  * @todo need to make sure validation is run before adding files
  */
  
 // find the theme we are working with by examining the clicked element
 $button_name = $form_state['clicked_button']['#name'];
 $theme = str_replace('_new_bundle', '', $button_name);
 
 // get the info we need
 $list = $form_state['values'][$theme . '_list'];
 $name = $form_state['values'][$theme . '_css_bundle_name'];
 $paths = $form_state['values'][$theme . '_css_bundle_paths'];
 $files = array();

 foreach($list as $file){
   if($file){
     array_push($files, $file);
   }
 }
 
 
 // create a keyed arrayto store in the DB
 $name_index = drupal_html_id($name);
 $bundle_details = array(
   'name' => $name,
   'files' => $files,
   'paths' => $paths
 );
 
 print_r($bundle_details, 'bindle details');
 die;
 // make the theme theme index
 $bundle_index = $theme . '_drab_css_bundles';
 
 
 // each theme gets it's own key and the name is used as a subkey. this should 
 // allow us easily overwrite bundles without too much hassle
 $updated_info = array();
 $updated_info[$theme][$name_index][$bundle_details];
 variable_set_value($bundle_index, $updated_info);
 
 // update the form
 $html_id = $theme . '_drab_css_bundles'; 
 $form_index = $form[$theme]['css']['bundles']['existing'][$html_id];
 $commands[] = ajax_command_replace("#" . $html_id, drupal_render($form_index));
 
 return $commands;

}


/**
 * Helper function to validate that we have all the information to create a bundle
 */
function drab_bundle_form_validate(&$form, &$form_state) {
  
  // find the theme we are working with by examining the clicked element
 $button_name = $form_state['clicked_button']['#name'];
 $theme = str_replace('_new_bundle', '', $button_name);
 
 /**
  * Check that we have selected some files
  */
 $list = $form_state['values'][$theme . '_list'];
 $files = array();
 foreach($list as $file){
   if($file){
     array_push($files, $file);
   }
 }
 if(count($files) == 0) {
   form_set_error($theme . '_list', t('Please select a file'));
 }
 
 /**
  * Check we have a name for the bundle
  */
 $name = $form_state['values'][$theme . '_css_bundle_name'];
  
 if(empty($name)){
   form_set_error($name, t('Please enter a bundle name'));
 }
  dsm('validation run');
  
  dsm($name, 'name');

  dsm($file, 'files');
  
}

function drab_bundle_form_submit($form, &$form_state){
  // validate the form
  drupal_validate_form('drab_bundle', $form, $form_state);
  
  dsm('submit called');
  
  // if there are errors, return the form to display the error messages
  if (form_get_errors()) {
    $form_state['rebuild'] = TRUE;
    return $form; 
  }
}

/**
 * Generates markup to show the bundles
 * 
 * Returns a renderable array to usin the form api
 * 
 * @param type $data
 * @return array
 */
function _drab_generate_existing_bundles_table($data, $theme){
  
  $table_head = array(
    t('Bundle name'),
    t('Operations')
  );
  
  
  dsm($data, 'data');
       
  return 'bob';
}