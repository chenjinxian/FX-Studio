CREATE TABLE blog_post
       ("id" INTEGER PRIMARY KEY NOT NULL,
       "title" VARCHAR(255) DEFAULT NULL,
       "body" TEXT DEFAULT NULL,
       "n_comments" INTEGER DEFAULT NULL,
       "published_at" DATETIME DEFAULT NULL);

CREATE TABLE blog_comment
       ("id" INTEGER PRIMARY KEY NOT NULL,
       "post_id" INTEGER DEFAULT NULL,
       "author" VARCHAR(255) DEFAULT NULL,
       "email" VARCHAR(255) DEFAULT NULL,
       "url" VARCHAR(255) DEFAULT NULL,
       "body" TEXT DEFAULT NULL,
       "created_at" DATETIME DEFAULT NULL);

CREATE TABLE blog_page
       ("id" INTEGER PRIMARY KEY NOT NULL,
       "title" VARCHAR(30) DEFAULT NULL,
       "body" TEXT DEFAULT NULL);
