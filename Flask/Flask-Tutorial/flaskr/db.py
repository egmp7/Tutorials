import sqlite3

import click
from flask import current_app, g

""" Connect to database
@return dabase connection"""
def get_db():
    if 'db' not in g:
        g.db = sqlite3.connect(
            current_app.config['DATABASE'],
            detect_types=sqlite3.PARSE_DECLTYPES
        )
        g.db.row_factory = sqlite3.Row

    return g.db

""" Close database"""
def close_db(e=None):
    db = g.pop('db', None)

    if db is not None:
        db.close()


""" Create Schema"""
def init_db():
    db = get_db()

    with current_app.open_resource('schema.sql') as f:
        db.executescript(f.read().decode('utf8'))


""" Database init"""
@click.command('init-db')
def init_db_command():
    """Clear the existing data and create new tables."""
    init_db()
    click.echo('Initialized the database.')

""" Add command to the app"""
def init_app(app):
    app.teardown_appcontext(close_db)
    app.cli.add_command(init_db_command)