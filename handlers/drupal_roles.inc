<?php

class PumWebformExportDrupalRolesHandler {
  
  public function __construct() {
  }
  
  public static function export(&$node, $original_node) {
    if (isset($node->webform['roles']) && is_array($node->webform['roles'])) {
      foreach($node->webform['roles'] as $i => $role_id) {
        $role = user_role_load($role_id);
        if ($role) {
          $node->webform['roles'][$i] = $role->name;
        } else {
          unset($node->webform['roles'][$i]);
        }
      }
    }
    
    if (isset($node->node_privacy_byrole['roles']) && is_array($node->node_privacy_byrole['roles'])) {
      $roles = array();
      foreach($node->node_privacy_byrole['roles'] as $role_id => $node_privacy) {
        if ($node_privacy['view'] || $node_privacy['edit'] || $node_privacy['delete']) {
          $role = user_role_load($role_id);
          if ($role) {
            $roles[$role->name] = $node_privacy;
          }
        }
      }
      $node->node_privacy_byrole['roles'] = $roles;
    }
  }
  
  public static function import(&$node, $original_node) {
    if (isset($node->webform['roles']) && is_array($node->webform['roles'])) {
      foreach($node->webform['roles'] as $i => $role_name) {
        $role = false;
        if (is_string($role_name)) {
          $role = user_role_load_by_name($role_name);
        } elseif (is_int($role_name)) {
          $role = user_role_load($role_name);
        }
        
        if ($role) {
          $node->webform['roles'] [$i] = $role->rid;
        } else {
          throw new Exception('Could not find drupal role: '.$role_name);
        }
      }
    }
    
    if (isset($node->node_privacy_byrole['roles']) && is_array($node->node_privacy_byrole['roles'])) {
      
      $roles = array();
      $role_ids = array_keys(user_roles());
      foreach($role_ids as $rid) {
        $roles[$rid] = array('view' => 0, 'edit' => 0, 'delete' => 0);
      }
      
      foreach($node->node_privacy_byrole['roles'] as $role_name => $node_privacy) {
        $role = false;
        if (is_string($role_name)) {
          $role = user_role_load_by_name($role_name);
        } elseif (is_int($role_name)) {
          $role = user_role_load($role_name);
        }
        
        if ($role) {
          $roles[$role->rid] = $node_privacy;
        } else {
          throw new Exception('Could not find drupal role: '.$role_name);
        }
      }
      
      $node->node_privacy_byrole['roles'] = $roles;
    }
  }

  
}